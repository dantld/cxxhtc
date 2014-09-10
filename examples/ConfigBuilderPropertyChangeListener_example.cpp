     void onChangeProperty( const std::string& k, std::string& v) throw(ConfigBuilderListenerError)
     {
       if( k == "object.protocol.file.name" ) {
         if ( v.at(0) == '.' )
           v = kisc::ClientSysname + v;
       } else if( k == "object.monitor.file.name" ) {
         if ( v.at(0) == '.' )
           v = kisc::ClientSysname + v;
         protocol->logInfo(LOGSRC,  "Set monitor shm path to %s", v.c_str() );
       } else if ( k.find("io_catalog") != string::npos ) {
         v = kisc::ClientSysname + "/" + v;
       }
     }
