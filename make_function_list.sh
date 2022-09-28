#!/bin/bash

printf "## vecs\n" > function_list.md
echo "\`\`\`C" >> function_list.md
cat function_list.txt \
  | grep -w Vec_T \
  | sed 's/; */;\n/g' \
  | /usr/bin/sort -k3 \
  | sed 's/extern //g' \
  | awk 'NF' \
  >> function_list.md
echo "\`\`\`" >> function_list.md
printf "\n## maps\n" >> function_list.md
echo "\`\`\`C" >> function_list.md
cat function_list.txt \
  | grep -w Map_K__V \
  | sed 's/; */;\n/g' \
  | /usr/bin/sort -k3 \
  | sed 's/extern //g' \
  | awk 'NF' \
  >> function_list.md
echo "\`\`\`" >> function_list.md
