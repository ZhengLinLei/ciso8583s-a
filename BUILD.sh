docker build -t ciso8583_ots_a_engine_build -f docker/compile.dockerfile ./
docker run --rm -v $(pwd)/tmp:/opt/ciso8583/bin ciso8583_ots_a_engine_build

# All binaries in /tmp
# Run docker production.dockerfile
docker build -t ciso8583_ots_a -f docker/production.dockerfile ./

# Volume for logs
docker volume create ciso8583_ots_a_logs

rm -rf tmp

echo "---- Build finished ---"