#ifndef SNETWORK_H
#define SNETWORK_H

#include <QTimer>
#include <QWidget>
#include <QtNetwork>

class SNetwork : public QWidget
{
    Q_OBJECT
public:
    SNetwork( QWidget* parent = nullptr );

public slots:
    void setUrl( QString str );
    void open( QString type, QString path );
    void setRequestHeader( QString name, QString value );
    void send( QString data = "" );
    QString waitAnswer();
    void getResponse( QNetworkReply* );

private:
    QUrl url;

    QString answerflag;
    QString type;
    QString path;
    QNetworkRequest request;
    QVector<QVector<QString>> headers;
    QNetworkAccessManager* manager;
};

#endif // SNETWORK_H
