 /*
 * Краткий пример использования
 * динамического загрузчика.
 * При компоновке модуля важно
 * не забыть экспортировать
 * два символа: "create" и "destroy".
 */

/*
 * MyClass - реализация интерфейса SomeInterface
 */
class MyClass : public SomeInterface
{
public:
    // замещение (override) метода someMethod в SomeInterface
    void someMethod()
    {
    }
};

/*
 * Это реализация фабрики AbstractFactory
 * для создания класса MyClass.
 */

class MyClassFactory : public AbstractFactory
{
    virtual void* createInstance( const char* name, Config* config ) throw(AbstractFactoryError)
    {
	return new MyClass();
    }
    virtual void  destroyInstance( void* object ) throw(AbstractFactoryError)
    {
	delete (MyClass*)object;
    }
};


/*
 * Точка входа для динамического загрузчика DynamicLoader.
 * Создание нового экземпляра фабрики MyClassFactory.
 */

extern "C" void* create()
{
    return new MyClassFactory();
}


/*
 * Точка входа для динамического загрузчика DynamicLoader.
 * Уничтожение экземпляра абстрактной библиотеки.
 * Здесь предполагается, что указатель на объект
 * указывает на экземпляр класса MyClassFactory.
 */

extern "C" void destroy( void* object )
{
    delete (MyClassFactory*)object;
}
