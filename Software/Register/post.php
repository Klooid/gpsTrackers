<?php 
	
	/*
		CONECTAR AL SERVIDOR MYSQL
	*/

	$server_link = mysqli_connect($server, $dbuser, $dbpass, $dbname);
	/* cambiar el conjunto de caracteres a utf8 */

	mysqli_set_charset($server_link, "utf8");


	if (mysqli_connect_errno()) {
	    printf("Falló la conexión: %s\n", mysqli_connect_error());
	    exit();
	}

	if(isset($_GET['device']) && isset($_GET['lat']) && isset($_GET['long']) && isset($_GET['alt']) && isset($_GET['speed']) && isset($_GET['dir']) && isset($_GET['status']) )
	{
		$sql = "INSERT INTO trackers (device, longitude, latitude, altitude, speed, direction, peripheral_status) VALUES (".$_GET['device'].",".$_GET['long'].",".$_GET['lat'].",".$_GET['alt'].",".$_GET['speed'].",".$_GET['dir'].",".$_GET['status'].")";

				if (mysqli_query($server_link,$sql)) {
				    echo "DONE";
				} else {
				    echo "FAILED";
				}

		mysqli_close($server_link);

	}
	else
		echo "No hay definicion del parametro valor";
 ?>

