echo "Running binary"

./bin/ciso8583 >> ./log/Ciso8583.log 2>&1 &

# tail -f ./log/Ciso8583.log