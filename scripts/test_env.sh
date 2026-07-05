#!/bin/bash
# test_env.sh
# Prepara un ambiente di test per lo scanner:
#   - 3 porte aperte (nc in background)
#   - 3 porte filtrate (iptables DROP)
#   - il resto chiuse per default
# Tutto viene ripulito automaticamente all'uscita (anche su Ctrl+C).

SCANNER="./bin/port_scanner"
TARGET="127.0.0.1"
RANGE="60000-60100"

OPEN_PORTS=(60010 60050 60090)
DROP_PORTS=(60020 60060 60080)

NC_PIDS=()

cleanup() {
    echo ""
    echo "Pulizia in corso..."

    for pid in "${NC_PIDS[@]}"; do
        kill "$pid" 2>/dev/null
    done

    for port in "${DROP_PORTS[@]}"; do
        sudo iptables -D INPUT -p tcp --dport "$port" -j DROP 2>/dev/null
    done

    echo "Pulizia completata."
}

trap cleanup EXIT

# Apri porte con nc in background
echo "Apertura porte: ${OPEN_PORTS[*]}"
for port in "${OPEN_PORTS[@]}"; do
    nc -l -p "$port" &
    NC_PIDS+=($!)
done

# Aggiungi regole DROP
echo "Filtro porte (DROP): ${DROP_PORTS[*]}"
for port in "${DROP_PORTS[@]}"; do
    sudo iptables -A INPUT -p tcp --dport "$port" -j DROP
done

echo ""
echo "Ambiente pronto. Avvio scanner: $SCANNER $TARGET $RANGE"
echo "Atteso: 60010, 60050, 60090 aperte — 60020, 60060, 60080 filtrate — resto chiuse"
echo "---"
$SCANNER "$TARGET" "$RANGE"
echo "---"
echo "Scan completato."
