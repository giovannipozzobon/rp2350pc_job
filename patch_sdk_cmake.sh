#!/bin/bash
# Patch per aggiornare CMakeLists.txt obsoleti nel pico-sdk
# Compatibile con macOS, Linux, e WSL

# Percorso al pico-sdk (modifica se diverso)
PICO_SDK_PATH="${HOME}/Documents/pico/pico-sdk"

echo "🔧 Patching pico-sdk in: $PICO_SDK_PATH"

# Cerca tutti i file CMakeLists.txt con 'cmake_minimum_required(VERSION 2.8)'
find "$PICO_SDK_PATH" -name CMakeLists.txt | while read file; do
  if grep -q 'cmake_minimum_required(VERSION 2.8)' "$file"; then
    echo "📄 Trovato file da patchare: $file"
    # Sostituisci 2.8 con 3.5
    sed -i.bak 's/cmake_minimum_required(VERSION 2.8)/cmake_minimum_required(VERSION 3.5)/' "$file"
    echo "✅ Patch applicata a $file"
  fi
done

echo "✅ Tutti i file patchati."