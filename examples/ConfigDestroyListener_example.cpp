
    bool onDestroyObject(
        Config *c,
        const std::string& objectName,
        void* obj,
        AbstractFactory *factory
    ) throw(ConfigDestroyListenerError)
    {
	// Этот объект был создан без стандартной фабрики из конфигурационного объекта.
        if( objectName == "rwsync.payment" ) {
    	    // Применение фабрики, заданной пользователем, для уничтожения объекта.
            ReadWriteSyncFactory *f = ReadWriteSync::getDefaultFactory();
            f->destroy( (ReadWriteSync*)obj );
            delete f;
            // Отчёт для конфигурационного объекта, что объект 
            // уничтожен (от listener).
            return true;
        } else if( objectName == "threadpool" ) {
    	    // Выполнение специальных действий перед
    	    // уничтожением пула потоков (thread pool).
    	    // Перед началом уничтожения мы должны убедиться, 
    	    // что не используются никакие ресурсы 
    	    // подключений (connection resource).
            waitForFreeConnections( c );
            // Отчёт для конфигурационного объекта, 
            // что мы не уничтожаем объект.
            return false;
        }
        // Уведомление для всех прочих объектов, 
        // что мы не вмешиваемся в процесс уничтожения.
        return false;
    }
