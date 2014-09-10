
char property_re = "^%s:(.+)$";
char reBuf[256];
char propVal[128];
snprintf( reBuf, 256, property_re, "timeout" );
RegExp re( reBuf );
if( re.match( buffer, 1, propVal, 128 ) ) {
    // do something with property value.
} else {
    // report error.
}

