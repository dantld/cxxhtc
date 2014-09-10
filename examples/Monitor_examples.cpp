

	    MonitorServer *monSrv  = (MonitorServer*)config->getObject( OBJ_MONITOR_SRV );
	    if( argc == 1 ) {
		while( !signalled ) {
		    sleep(1);
		    if(monSrv) monSrv->checkpoint();
		}
	    }
