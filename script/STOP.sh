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

if [ "$($DOCKER ps -a -q -f name=ciso8583_ots_a)" ]; then
    echo "Stopping container"
    $DOCKER container stop ciso8583_ots_a
    $DOCKER container remove ciso8583_ots_a || $DOCKER container rm ciso8583_ots_a
else
    echo "Container already stopped"
fi