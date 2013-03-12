#ifndef STRINGHELPER_H
#define STRINGHELPER_H

#include <QObject>
#include <QString>

class ServerSelector : public QObject
{
    Q_OBJECT
public:
    ServerSelector(QObject *parent = 0) : QObject(parent), reverse(false) { }

    Q_INVOKABLE QString echo(const QString &text) const {
        if(reverse == false) { return text; }

        QString reversed;
        for(QString::const_iterator it = text.begin(); it != text.end(); it++) {
            reversed.push_front(*it);
        }

        return reversed;
    }

public slots:
    void toggleEcho(bool reverse) { this->reverse = reverse; }

protected:
    bool reverse;
};

#endif // STRINGHELPER_H
