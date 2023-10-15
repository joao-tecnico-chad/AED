#!/bin/bash

# Navegar até o diretório especificado
cd /home/john31704/IST/AED/LAB5/tests/

# Executar o programa em todos os arquivos .txt
for file in *.txt; do
    ./qsortwords "$file"
done

# Comprimir todos os arquivos resultantes em um arquivo .zip
zip compressed_results.zip *.palavras

# (Opcional) Remover os arquivos .palavras após a compressão, se desejar
# rm *.palavras