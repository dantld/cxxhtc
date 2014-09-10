
const char* const DIVBY_NONE_STR  = "none";
const char* const DIVBY_DAY_STR   = "day";
const char* const DIVBY_WEEK_STR  = "week";
const char* const DIVBY_MONTH_STR = "month";

SomeTask::SomeTask( Config *config ) 
{
    ConfigHelper ch(config);
    _name_file = _config->getProp("protocol.file.name");
    _divby_name_path = _name_path = _config->getProp("protocol.save.path");
    _create_path = ch.getBool("protocol.create.path",false);
    _file_mode = ch.getOct( "protocol.file.mode",0600);
    if( (_file_mode & 0600) == 0 ) _file_mode |= 0600;
    _dir_mode  = ch.getOct( "protocol.dir.mode", 0700 );
    if( (_dir_mode & 0700) == 0 ) _dir_mode |= 0700;
    _overwrite =  ch.getBool("protocol.overwrite.file",false);
    _filter_string = ch.getString("protocol.filter.string","*.error;*.warning;*.info;");
    string divby_str = ch.getString("protocol.divide.by",
        DIVBY_DAY_STR,
        DefaultValues(DIVBY_NONE_STR, DIVBY_DAY_STR, DIVBY_WEEK_STR, DIVBY_MONTH_STR, 0)
    );
}
