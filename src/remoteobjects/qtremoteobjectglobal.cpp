/****************************************************************************
**
** Copyright (C) 2014 Ford Motor Company
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtRemoteObjects module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qtremoteobjectglobal.h"

#include <QDataStream>
#include <QMetaObject>
#include <QMetaProperty>

QT_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(QT_REMOTEOBJECT, "qt.remoteobjects", QtWarningMsg)
Q_LOGGING_CATEGORY(QT_REMOTEOBJECT_MODELS, "qt.remoteobjects.models", QtWarningMsg)
Q_LOGGING_CATEGORY(QT_REMOTEOBJECT_IO, "qt.remoteobjects.io", QtWarningMsg)

namespace QtRemoteObjects {

void copyStoredProperties(const QMetaObject *mo, const void *src, void *dst)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 5, 0))
    if (!src) {
        qCWarning(QT_REMOTEOBJECT) << Q_FUNC_INFO << ": trying to copy from a null source";
        return;
    }
    if (!dst) {
        qCWarning(QT_REMOTEOBJECT) << Q_FUNC_INFO << ": trying to copy to a null destination";
        return;
    }

    for (int i = 0, end = mo->propertyCount(); i != end; ++i) {
        const QMetaProperty mp = mo->property(i);
        mp.writeOnGadget(dst, mp.readOnGadget(src));
    }
#else
    Q_UNUSED(mo);
    Q_UNUSED(src);
    Q_UNUSED(dst);
#endif
}

void copyStoredProperties(const QMetaObject *mo, const void *src, QDataStream &dst)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 5, 0))
    if (!src) {
        qCWarning(QT_REMOTEOBJECT) << Q_FUNC_INFO << ": trying to copy from a null source";
        return;
    }

    for (int i = 0, end = mo->propertyCount(); i != end; ++i) {
        const QMetaProperty mp = mo->property(i);
        dst << mp.readOnGadget(src);
    }
#else
    Q_UNUSED(mo);
    Q_UNUSED(src);
    Q_UNUSED(dst);
#endif
}

void copyStoredProperties(const QMetaObject *mo, QDataStream &src, void *dst)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 5, 0))
    if (!dst) {
        qCWarning(QT_REMOTEOBJECT) << Q_FUNC_INFO << ": trying to copy to a null destination";
        return;
    }

    for (int i = 0, end = mo->propertyCount(); i != end; ++i) {
        const QMetaProperty mp = mo->property(i);
        QVariant v;
        src >> v;
        mp.writeOnGadget(dst, v);
    }
#else
    Q_UNUSED(mo);
    Q_UNUSED(src);
    Q_UNUSED(dst);
#endif
}

} // namespace QtRemoteObjects

QT_END_NAMESPACE
