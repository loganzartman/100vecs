#!/usr/bin/env python3

from random import choice, randint
import subprocess

from requests import delete


def runner(body):
  return f"""
  #include "100vecs.h"
  #include <stdio.h>

  MAP_IMPL(int, int)

  int hash_int(int i) {{
    return i;
  }}
  bool eq_int(int a, int b) {{
    return a == b;
  }}

  int main(int argc, char const* argv[]) {{
    {body}
  }}
  """


def c_literal(x):
  if isinstance(x, str):
    return f'"{x}"'
  if isinstance(x, (int, float)):
    return f'{x}'
  raise Exception(f"Unsupported literal: {type(x)}")


def run_code(code):
  with open("fuzzer_gen.c", "w+") as f:
    f.write(code)
  subprocess.run(["make", "fuzzer_gen"], check=True)
  result_bytes = subprocess.check_output(["./fuzzer_gen"])
  result = result_bytes.decode('utf-8')
  return result.splitlines()


class MapSimulator:
  def __init__(self):
    self.type_k = "int"
    self.type_v = "int"
    self.map_id = 0
    self.statements = []
    self.model = {}
    self.expectations = []

  def map_type(self):
    return f"Map_{self.type_k}__{self.type_v}"

  def map_raw_op(self, op, *args):
    return f"map_{op}_{self.type_k}__{self.type_v}({', '.join(args)})"

  def map_op(self, op, *args):
    return self.map_raw_op(op, f"m{self.map_id}", *args)

  def create(self):
    decl = f"{self.map_type()}* m{self.map_id}"
    op = self.map_raw_op("create", "hash_int", "eq_int")
    self.statements.append(f"{decl} = {op};")
    self.model = {}

  def put(self, k, v):
    self.statements.append(self.map_op(
        "put", c_literal(k), c_literal(v)) + ";")
    self.model[k] = v

  def delete(self, k):
    self.statements.append(self.map_op("delete", c_literal(k)) + ";")
    self.model.pop(k)

  def get(self, k):
    self.statements.append(
        f"/* {len(self.expectations)} */ printf(\"%d\\n\", *{self.map_op('get', c_literal(k))});")
    self.expectations.append(self.model[k])

  def render(self):
    return runner('\n'.join(self.statements))

  def run_and_assert(self):
    results = run_code(self.render())
    for i, (actual, expected) in enumerate(zip(results, self.expectations)):
      if actual != str(expected):
        raise AssertionError(
            f"Checkpoint {i}: expected: {str(expected)}, actual: {actual}")


def main():
  map_sim = MapSimulator()
  map_sim.create()
  for _ in range(1000):
    r = randint(0, 2)
    if r == 0:
      map_sim.put(randint(0, 1000), randint(0, 1000))
    if r == 1 and len(map_sim.model):
      map_sim.delete(choice(list(map_sim.model.keys())))
    if r == 2 and len(map_sim.model):
      map_sim.get(choice(list(map_sim.model.keys())))
  map_sim.run_and_assert()


if __name__ == "__main__":
  main()
