# Run container cis08583_ots_a end expose all ports with --netword=host
docker run -d --name ciso8583_ots_a --network=host -v ciso8583_ots_a_logs:/opt/ciso8583/logs ciso8583_ots_a