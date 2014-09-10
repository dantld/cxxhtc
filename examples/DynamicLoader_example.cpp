// Объявить typedef загрузчика фабрики AbstractFactory.
typedef DynamicLoader<AbstractFactory> FactoryLoader;
// Создать экземпляр динамического загрузчика и загрузить динамический объект "my-module-name.so".
FactoryLoader loader("my-module-name.so");
// Создать экземпляр интерфейса фабрики AbstractFactory.
AbstarctFactory *factory = loader.create();
// Создать продукт фабрики (экземпляр некоторого SomeInterface ).
SomeInterface* object = (SomeInterface*)factory->createInstance( "my.object", config );
// Сделать что-либо с помощью экземпляра SomeInterface.
object->someMethod();
// Уничтожить продукт фабрики.
factory->destroyInstance( object );
// Уничтожить фабрику.
loader.destroy(factory);
// Загрузчик уничтожается автоматически при выходе из области видимости.

