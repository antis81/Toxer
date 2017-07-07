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

#include "Settings.h"

QVector<ToxSettings*> ToxSettings::notifiers = {};
QVector<UiSettings*> UiSettings::notifiers = {};

void Settings::registerQmlTypes()
{
    constexpr const char* qmlModule = { "com.toxer.settings" };
    qmlRegisterType<ToxSettings>(qmlModule, 1, 0, "ToxSettings");
    qmlRegisterType<UiSettings>(qmlModule, 1, 0, "UiSettings");
}

Settings::Settings(QSettings::Scope scope, const QString& filename)
    : QSettings(QSettings::IniFormat, scope, QStringLiteral("Toxer"), filename)
{
    switch (scope) {
    case QSettings::SystemScope:
        setObjectName(QStringLiteral("ToxerSystemSettings"));
        break;
    case QSettings::UserScope:
        setObjectName(QStringLiteral("ToxerUserSettings"));
        break;
    }
}

ToxSettings::ToxSettings(QSettings::Scope scope)
    : Settings(scope)
{
    ToxSettings::notifiers.append(this);
}

ToxSettings::~ToxSettings()
{
    ToxSettings::notifiers.removeAll(this);
}

bool ToxSettings::ipv6_enabled() const {
    return value(QLatin1String("tox/ipv6_enabled"), true).toBool();
}

void ToxSettings::set_ipv6_enabled(bool enabled) {
    if (set(QLatin1String("tox/ipv6_enabled"), enabled, ipv6_enabled())) {
        foreach (ToxSettings* n, ToxSettings::notifiers) {
            emit n->ipv6_enabled_changed(enabled);
        }
    }
}

bool ToxSettings::udp_enabled() const {
    return value(QLatin1String("tox/udp_enabled"), false).toBool();
}

void ToxSettings::set_udp_enabled(bool enabled) {
    if (set(QLatin1String("tox/udp_enabled"), enabled, udp_enabled())) {
        foreach (ToxSettings* n, ToxSettings::notifiers) {
            emit n->udp_enabled_changed(enabled);
        }
    }
}

ToxTypes::Proxy ToxSettings::proxy_type() const {
    return value(QLatin1String("tox/proxy_type"),
                 ToxTypes::toQVariant(ToxTypes::Proxy::None))
            .value<ToxTypes::Proxy>();
}

void ToxSettings::set_proxy_type(ToxTypes::Proxy type) {
    if (set(QLatin1String("tox/proxy_type"), ToxTypes::toQVariant(type),
            ToxTypes::toQVariant(proxy_type())))
    {
        foreach (ToxSettings* n, ToxSettings::notifiers) {
            emit proxy_type_changed(type);
        }
    }
}

quint16 ToxSettings::proxy_port() const {
    return static_cast<quint16>(
                value(QLatin1String("tox/proxy_port"), 0).toUInt());
}

void ToxSettings::set_proxy_port(quint16 port) {
    if (set(QLatin1String("tox/proxy_port"), port, proxy_port())) {
        foreach (ToxSettings* n, ToxSettings::notifiers) {
            emit n->proxy_port_changed(port);
        }
    }
}

QString ToxSettings::proxy_addr() const {
    return value(QLatin1String("tox/proxy_addr")).toString();
}

void ToxSettings::set_proxy_addr(const QString& ip) {
    if (set(QLatin1String("tox/proxy_addr"), ip, proxy_addr())) {
        foreach (ToxSettings* n, ToxSettings::notifiers) {
            emit n->proxy_addr_changed(ip);
        }
    }
}

UiSettings::UiSettings(QSettings::Scope scope)
    : Settings(scope)
{
    UiSettings::notifiers.append(this);
}

UiSettings::~UiSettings()
{
    UiSettings::notifiers.removeAll(this);
}

UiSettings::AppLayout UiSettings::app_layout() const
{
    QVariant v = value(QLatin1String("ui/app_layout"),
                       ToxTypes::enumKey(AppLayout::Split));
    return ToxTypes::enumValue<AppLayout>(v.toByteArray());
}

void UiSettings::set_app_layout(UiSettings::AppLayout layout)
{
    if (set(QLatin1String("ui/app_layout"), ToxTypes::enumKey(layout),
            ToxTypes::enumKey(app_layout())))
    {
        foreach (UiSettings* n, UiSettings::notifiers) {
            emit n->appLayoutChanged(static_cast<quint8>(layout));
        }
    }
}

quint8 UiSettings::app_layout_int() const
{
    return static_cast<quint8>(app_layout());
}

void UiSettings::set_app_layout(quint8 layout)
{
    set_app_layout(static_cast<AppLayout>(layout));
}

bool UiSettings::fullscreen() const
{
    return value(QLatin1String("ui/fullscreen")).toBool();
}

void UiSettings::set_fullscreen(bool enabled)
{
    if (set(QLatin1String("ui/fullscreen"), enabled, fullscreen())) {
        foreach (UiSettings* n, UiSettings::notifiers) {
            emit n->fullscreenChanged(enabled);
        }
    }
}

QRect UiSettings::geometry() const
{
    return value(QLatin1String("ui/geometry")).toRect();
}

void UiSettings::set_geometry(const QRect& rect)
{
    if (set(QLatin1String("ui/geometry"), rect, geometry())) {
        foreach (UiSettings* n, UiSettings::notifiers) {
            emit geometryChanged(rect);
        }
    }
}
