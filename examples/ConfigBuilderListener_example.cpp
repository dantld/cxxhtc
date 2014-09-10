namespace {
class MyCfgBldLsnr : public ConfigBuilderListener
{
public:
    MyCfgBldLsnr() throw() {}
    ~MyCfgBldLsnr() throw() {}
    void* beforeBuildObject(
        Config *c,
        const std::string objectName,
        AbstractFactory *factory
    ) throw(ConfigBuilderListenerError)
    {
        if( objectName == "rwsync.payment" ) {
            ReadWriteSyncFactory *f = ReadWriteSync::getDefaultFactory();
            void *obj = f->create( c );
            delete f;
            return obj;
        }
        return NULL;
    }
    void afterBuildObject(
        Config *c,
        const std::string objectName,
        void* obj
    ) throw(ConfigBuilderListenerError)
    {
        if( objectName == "oraenv" ) {
            custom_register_mapping( c, obj );
        }
    }
    void onProperty(
        const std::string &k,
        const std::string &v
        ) throw( ConfigBuilderListenerError)
    {
        if( k == "app.format.number.sequence.to_text" ) {
            format::NumberToText = v;
        } else if( k == "app.format.number.swift.numeric.nls" ) {
            format::NumberSwiftNumericNLS  = v;
        } else if( k == "app.format.number.swift.to_money" ) {
            format::NumberSwiftToMoney = v;
        }
    }
};
