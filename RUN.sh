# Run container cis08583_ots_a end expose all ports with --netword=host
container_name=ciso8583_ots_a

# Add custom flags
flag=
if [ "$#" -eq 1 ]; then
    flag=$1
    echo "Flags: $flag"
fi

# Execute
if [ "$(docker ps -aq -f status=exited -f name=$container_name)" ] || [ "$(docker ps -aq -f status=created -f name=$container_name)" ]; then
    echo "Container $container_name exists but is not running"
    echo "Removing container $container_name to relaunch it"
    # cleanup
    ./STOP.sh
    echo -e "-------------------\n\n"
fi
if [ ! "$(docker ps -a -q -f name=$container_name)" ]; then
    echo "Running $container_name"

    if [ "$(uname)" == "Darwin" ]; then
        # For testing purposes (Mac)
        echo "Mac OS"
        docker run $flag -it --name $container_name -p 9101:9101 -v ciso8583_ots_a_logs:/opt/Ciso8583/log $container_name
    elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
        # For production (Linux)
        echo "Linux OS"
        docker run $flag -it --name $container_name --network=host -v ciso8583_ots_a_logs:/opt/Ciso8583/log $container_name
        # docker run $flag -it --name $container_name -p 9101:9101 -v ciso8583_ots_a_logs:/opt/Ciso8583/log $container_name
    else
        echo "Unsupported OS"
    fi
fi
docker container exec -it $container_name bash