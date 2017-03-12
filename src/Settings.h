/*
 * This file is part of the Toxer application, a Tox messenger client.
 *
 * Copyright (c) 2017 Nils Fenner <nils@macgitver.org>
 *
 * This software is licensed under the terms of the MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef TOXER_SETTINGS_H
#define TOXER_SETTINGS_H

#include <ToxTypes.h>

#include <QSettings>

class Settings : public QSettings
{
public:
    static void registerQmlTypes();

protected:
    Settings(Scope scope, const QString& filename = QStringLiteral("Settings"));

    inline bool set(const QLatin1String& key, const QVariant& newValue,
                    const QVariant& oldValue)
    {
        if (newValue == oldValue) {
            // unchanged
            return false;
        } else {
            setValue(key, newValue);
            return true;
        }
    }
};

class ToxSettings : public Settings
{
    Q_OBJECT
public:
    ToxSettings(Scope scope = UserScope);

    Q_INVOKABLE bool ipv6_enabled() const;
    Q_INVOKABLE void set_ipv6_enabled(bool enabled);

    Q_INVOKABLE bool udp_enabled() const;
    Q_INVOKABLE void set_udp_enabled(bool enabled);

    Q_INVOKABLE ToxTypes::Proxy proxy_type() const;
    Q_INVOKABLE void set_proxy_type(ToxTypes::Proxy type);

    Q_INVOKABLE quint16 proxy_port() const;
    Q_INVOKABLE void set_proxy_port(quint16 port);

    Q_INVOKABLE QString proxy_addr() const;
    Q_INVOKABLE void set_proxy_addr(const QString& ip);

signals:
    void ipv6_enabled_changed(bool);
    void udp_enabled_changed(bool);
    void proxy_type_changed(ToxTypes::Proxy);
    void proxy_port_changed(quint16);
    void proxy_addr_changed(QString);
};

#endif
