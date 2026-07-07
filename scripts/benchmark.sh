#!/bin/bash
# benchmark.sh
# Aggiunge regole iptables DROP su porte selezionate nel range 60000-60100
# per simulare porte filtrate durante il benchmark dello scanner.
# Le regole vengono rimosse automaticamente all'uscita (anche su Ctrl+C).

PORTS=(60005 60010 60020 60030 60035 60040 60050 60055 60060 60070 60075 60080 60090 60095 60100)
SCANNER="./bin/port_scanner"
TARGET="127.0.0.1"
RANGE="60000-60100"

cleanup() {
    echo ""
    echo "Rimozione regole iptables..."
    for port in "${PORTS[@]}"; do
        sudo iptables -D INPUT -p tcp --dport "$port" -j DROP 2>/dev/null
    done
    echo "Pulizia completata."
}

trap cleanup EXIT

echo "Aggiunta regole iptables DROP su ${#PORTS[@]} porte..."
for port in "${PORTS[@]}"; do
    sudo iptables -A INPUT -p tcp --dport "$port" -j DROP
done
echo "Regole attive."
echo ""

echo "Avvio benchmark: $SCANNER $TARGET $RANGE"
echo "---"
$SCANNER "$TARGET" "$RANGE"
echo "---"
echo "Benchmark completato."
