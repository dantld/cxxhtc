{
    ConfigBuilder *b = kisc::config::XmlConfigBuilder::createInstance("application-config.xml");

    b->buildProperties();
    b->buildProtocol();
    b->buildFactories();
    b->buildObjects();
    b->buildTasks();

    Config *config = b->getConfig();
}
