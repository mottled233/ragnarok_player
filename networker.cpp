#include "networker.h"
class NetWorker::Private
{
public:
    Private(NetWorker *q) :
        manager(new QNetworkAccessManager(q))
    {}

    QNetworkAccessManager *manager;
};

NetWorker::NetWorker(QObject *parent) :
    QObject(parent),
    d(new NetWorker::Private(this))
{
    connect(d->manager, &QNetworkAccessManager::finished,
            this, &NetWorker::finished);
}
NetWorker *NetWorker::instance()
{
    static NetWorker netWorker;//声明了一个 static 变量，将其指针返回。这是 C++ 单例模式的最简单写法
    return &netWorker;
}

void NetWorker::get(const QString &url)
{
    d->manager->get(QNetworkRequest(QUrl(url)));
}

NetWorker::~NetWorker()
{
    delete d;
    d = 0;
}

