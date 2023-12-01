if [ "$(docker ps -a -q -f name=ciso8583_ots_a)" ]; then
    echo "Stopping container"
    docker container stop ciso8583_ots_a
    docker container remove ciso8583_ots_a
else
    echo "Container already stopped"
fi