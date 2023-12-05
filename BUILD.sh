# Check if $1 is "podman" or "docker" then use it
if [ "$1" == "podman" ]; then
    DOCKER=podman
    podman --version || {
        echo "Error: podman not found"
        exit 1
    }
else
    DOCKER=docker
    docker --version || {
        echo "Error: docker not found"
        exit 1
    }
fi


echo "Checking if container is running"
./STOP.sh

echo "---- Build started ---"
$DOCKER build -t ciso8583_ots_a_engine_build -f docker/compile.dockerfile ./
$DOCKER run --rm -v $(pwd)/tmp:/opt/Ciso8583/tmp ciso8583_ots_a_engine_build

# All binaries in /tmp
# Run docker production.dockerfile
$DOCKER build -t ciso8583_ots_a -f docker/production.dockerfile ./

# Volume for logs
$DOCKER volume create ciso8583_ots_a_logs

rm -rf tmp

yes | $DOCKER image prune

echo "---- Build finished ---"