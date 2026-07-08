#include "data/DatabaseManager.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QUuid>

namespace {
Conversation readConversation(QSqlQuery& query)
{
    Conversation c;
    c.id = query.value("id").toString();
    c.title = query.value("title").toString();
    c.avatarColor = query.value("avatar_color").toString();
    c.lastMessage = query.value("last_message").toString();
    c.lastTime = QDateTime::fromString(query.value("last_time").toString(), Qt::ISODate);
    c.unreadCount = query.value("unread_count").toInt();
    c.pinned = query.value("pinned").toInt() != 0;
    c.muted = query.value("muted").toInt() != 0;
    c.hidden = query.value("hidden").toInt() != 0;
    c.group = query.value("is_group").toInt() != 0;
    return c;
}
}

DatabaseManager::DatabaseManager(QObject* parent)
    : QObject(parent)
{
}

DatabaseManager::~DatabaseManager()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool DatabaseManager::initialize(const QString& databasePath)
{
    const QFileInfo info(databasePath);
    QDir().mkpath(info.absolutePath());

    const QString connectionName = QStringLiteral("wechat_demo_connection");
    if (QSqlDatabase::contains(connectionName)) {
        m_db = QSqlDatabase::database(connectionName);
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    }

    m_db.setDatabaseName(databasePath);
    if (!m_db.open()) {
        qWarning() << "打开数据库失败:" << m_db.lastError().text();
        return false;
    }

    return createSchema() && seedIfEmpty();
}

bool DatabaseManager::createSchema()
{
    const QStringList statements = {
        "CREATE TABLE IF NOT EXISTS conversations ("
        "id TEXT PRIMARY KEY, title TEXT NOT NULL, avatar_color TEXT, last_message TEXT, "
        "last_time TEXT, unread_count INTEGER DEFAULT 0, pinned INTEGER DEFAULT 0, "
        "muted INTEGER DEFAULT 0, hidden INTEGER DEFAULT 0, is_group INTEGER DEFAULT 0)",

        "CREATE TABLE IF NOT EXISTS contacts ("
        "id TEXT PRIMARY KEY, name TEXT NOT NULL, pinyin TEXT, group_name TEXT, "
        "avatar_color TEXT, status TEXT, is_group INTEGER DEFAULT 0)",

        "CREATE TABLE IF NOT EXISTS messages ("
        "id TEXT PRIMARY KEY, conversation_id TEXT NOT NULL, sender_id TEXT, sender_name TEXT, "
        "avatar_color TEXT, content TEXT, type TEXT, created_at TEXT, status TEXT, outgoing INTEGER DEFAULT 0)"
    };

    for (const QString& sql : statements) {
        if (!execSql(sql)) {
            return false;
        }
    }
    return true;
}

bool DatabaseManager::seedIfEmpty()
{
    QSqlQuery countQuery(m_db);
    if (!countQuery.exec("SELECT COUNT(*) FROM conversations") || !countQuery.next()) {
        return false;
    }
    if (countQuery.value(0).toInt() > 0) {
        return true;
    }

    struct SeedConversation {
        QString id;
        QString title;
        QString color;
        QString last;
        int unread;
        bool pinned;
        bool group;
    };

    const QVector<SeedConversation> convs = {
        {"c_team", "项目体验设计群", "#21bf73", "今晚 8 点同步一下新版交互走查结果", 6, true, true},
        {"c_lina", "林娜", "#5b8def", "收到，我把接口联调结果整理给你", 2, false, false},
        {"c_mike", "Mike", "#f59f00", "The desktop demo looks pretty smooth.", 0, false, false},
        {"c_file", "文件传输助手", "#8a8f98", "图片 wx_mockup.png", 0, false, false},
        {"c_family", "家人群", "#ff6b6b", "周末一起吃饭吗？", 3, false, true}
    };

    QSqlQuery convInsert(m_db);
    convInsert.prepare("INSERT INTO conversations VALUES(?,?,?,?,?,?,?,?,?,?)");
    for (int i = 0; i < convs.size(); ++i) {
        const auto& c = convs[i];
        convInsert.addBindValue(c.id);
        convInsert.addBindValue(c.title);
        convInsert.addBindValue(c.color);
        convInsert.addBindValue(c.last);
        convInsert.addBindValue(QDateTime::currentDateTime().addSecs(-3600 * i).toString(Qt::ISODate));
        convInsert.addBindValue(c.unread);
        convInsert.addBindValue(c.pinned ? 1 : 0);
        convInsert.addBindValue(0);
        convInsert.addBindValue(0);
        convInsert.addBindValue(c.group ? 1 : 0);
        if (!convInsert.exec()) {
            qWarning() << "写入会话种子数据失败:" << convInsert.lastError().text();
            return false;
        }
    }

    struct SeedContact {
        QString id;
        QString name;
        QString pinyin;
        QString group;
        QString color;
        QString status;
        bool isGroup;
    };
    const QVector<SeedContact> contacts = {
        {"new_friend", "新的朋友", "A", "入口", "#07c160", "查看好友申请", false},
        {"group_entry", "群聊", "A", "入口", "#14b8a6", "管理我的群聊", true},
        {"tag_entry", "标签", "A", "入口", "#64748b", "联系人标签", false},
        {"u_lina", "林娜", "L", "L", "#5b8def", "在线", false},
        {"u_mike", "Mike", "M", "M", "#f59f00", "忙碌", false},
        {"u_chen", "陈晨", "C", "C", "#ef4444", "今天状态很好", false},
        {"u_wang", "王一", "W", "W", "#8b5cf6", "移动端开发", false},
        {"g_team", "项目体验设计群", "X", "群聊", "#21bf73", "16 位成员", true}
    };

    QSqlQuery contactInsert(m_db);
    contactInsert.prepare("INSERT INTO contacts VALUES(?,?,?,?,?,?,?)");
    for (const auto& c : contacts) {
        contactInsert.addBindValue(c.id);
        contactInsert.addBindValue(c.name);
        contactInsert.addBindValue(c.pinyin);
        contactInsert.addBindValue(c.group);
        contactInsert.addBindValue(c.color);
        contactInsert.addBindValue(c.status);
        contactInsert.addBindValue(c.isGroup ? 1 : 0);
        if (!contactInsert.exec()) {
            qWarning() << "写入联系人种子数据失败:" << contactInsert.lastError().text();
            return false;
        }
    }

    const QDateTime now = QDateTime::currentDateTime();
    QVector<Message> messages = {
        {QUuid::createUuid().toString(QUuid::WithoutBraces), "c_team", "u_lina", "林娜", "#5b8def", "今天的三栏布局我已经对齐微信桌面端视觉，右侧信息面板也加好了。", MessageType::Text, now.addSecs(-520), MessageStatus::Read, false},
        {QUuid::createUuid().toString(QUuid::WithoutBraces), "c_team", "me", "我", "#07c160", "很好，消息气泡和会话右键菜单也要一起走查。", MessageType::Text, now.addSecs(-480), MessageStatus::Read, true},
        {QUuid::createUuid().toString(QUuid::WithoutBraces), "c_team", "u_mike", "Mike", "#f59f00", "今晚 8 点同步一下新版交互走查结果", MessageType::Text, now.addSecs(-300), MessageStatus::Sent, false},
        {QUuid::createUuid().toString(QUuid::WithoutBraces), "c_lina", "u_lina", "林娜", "#5b8def", "你看下这个搜索逻辑，最好能同时搜聊天记录。", MessageType::Text, now.addSecs(-840), MessageStatus::Read, false},
        {QUuid::createUuid().toString(QUuid::WithoutBraces), "c_lina", "me", "我", "#07c160", "收到，我把接口联调结果整理给你", MessageType::Text, now.addSecs(-620), MessageStatus::Read, true},
        {QUuid::createUuid().toString(QUuid::WithoutBraces), "c_file", "system", "文件传输助手", "#8a8f98", "图片 wx_mockup.png", MessageType::Image, now.addSecs(-950), MessageStatus::Sent, false}
    };

    for (const Message& message : messages) {
        if (!addMessage(message)) {
            return false;
        }
    }
    return true;
}

QVector<Conversation> DatabaseManager::conversations(const QString& keyword) const
{
    QVector<Conversation> result;
    QSqlQuery query(m_db);
    QString sql = "SELECT * FROM conversations WHERE hidden = 0";
    if (!keyword.trimmed().isEmpty()) {
        sql += " AND (title LIKE :kw OR last_message LIKE :kw)";
    }
    sql += " ORDER BY pinned DESC, datetime(last_time) DESC";
    query.prepare(sql);
    if (!keyword.trimmed().isEmpty()) {
        query.bindValue(":kw", "%" + keyword.trimmed() + "%");
    }
    if (!query.exec()) {
        qWarning() << "查询会话失败:" << query.lastError().text();
        return result;
    }
    while (query.next()) {
        result.append(readConversation(query));
    }
    return result;
}

QVector<Contact> DatabaseManager::contacts(const QString& keyword) const
{
    QVector<Contact> result;
    QSqlQuery query(m_db);
    QString sql = "SELECT * FROM contacts";
    if (!keyword.trimmed().isEmpty()) {
        sql += " WHERE name LIKE :kw OR pinyin LIKE :kw OR status LIKE :kw";
    }
    sql += " ORDER BY group_name, pinyin, name";
    query.prepare(sql);
    if (!keyword.trimmed().isEmpty()) {
        query.bindValue(":kw", "%" + keyword.trimmed() + "%");
    }
    if (!query.exec()) {
        qWarning() << "查询联系人失败:" << query.lastError().text();
        return result;
    }
    while (query.next()) {
        Contact c;
        c.id = query.value("id").toString();
        c.name = query.value("name").toString();
        c.pinyin = query.value("pinyin").toString();
        c.groupName = query.value("group_name").toString();
        c.avatarColor = query.value("avatar_color").toString();
        c.status = query.value("status").toString();
        c.isGroup = query.value("is_group").toInt() != 0;
        result.append(c);
    }
    return result;
}

QVector<Message> DatabaseManager::messagesForConversation(const QString& conversationId, const QString& keyword) const
{
    QVector<Message> result;
    QSqlQuery query(m_db);
    QString sql = "SELECT * FROM messages WHERE conversation_id = :conversation_id";
    if (!keyword.trimmed().isEmpty()) {
        sql += " AND content LIKE :kw";
    }
    sql += " ORDER BY datetime(created_at) ASC";
    query.prepare(sql);
    query.bindValue(":conversation_id", conversationId);
    if (!keyword.trimmed().isEmpty()) {
        query.bindValue(":kw", "%" + keyword.trimmed() + "%");
    }
    if (!query.exec()) {
        qWarning() << "查询消息失败:" << query.lastError().text();
        return result;
    }
    while (query.next()) {
        Message m;
        m.id = query.value("id").toString();
        m.conversationId = query.value("conversation_id").toString();
        m.senderId = query.value("sender_id").toString();
        m.senderName = query.value("sender_name").toString();
        m.avatarColor = query.value("avatar_color").toString();
        m.content = query.value("content").toString();
        m.type = messageTypeFromString(query.value("type").toString());
        m.timestamp = QDateTime::fromString(query.value("created_at").toString(), Qt::ISODate);
        m.status = messageStatusFromString(query.value("status").toString());
        m.outgoing = query.value("outgoing").toInt() != 0;
        result.append(m);
    }
    return result;
}

bool DatabaseManager::addMessage(const Message& message)
{
    QSqlQuery insert(m_db);
    insert.prepare("INSERT INTO messages VALUES(?,?,?,?,?,?,?,?,?,?)");
    insert.addBindValue(message.id);
    insert.addBindValue(message.conversationId);
    insert.addBindValue(message.senderId);
    insert.addBindValue(message.senderName);
    insert.addBindValue(message.avatarColor);
    insert.addBindValue(message.content);
    insert.addBindValue(messageTypeToString(message.type));
    insert.addBindValue(message.timestamp.toString(Qt::ISODate));
    insert.addBindValue(messageStatusToString(message.status));
    insert.addBindValue(message.outgoing ? 1 : 0);
    if (!insert.exec()) {
        qWarning() << "插入消息失败:" << insert.lastError().text();
        return false;
    }

    QSqlQuery update(m_db);
    update.prepare("UPDATE conversations SET last_message = ?, last_time = ?, unread_count = unread_count + ? WHERE id = ?");
    update.addBindValue(message.content);
    update.addBindValue(message.timestamp.toString(Qt::ISODate));
    update.addBindValue(message.outgoing ? 0 : 1);
    update.addBindValue(message.conversationId);
    if (!update.exec()) {
        qWarning() << "更新会话摘要失败:" << update.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::setConversationPinned(const QString& conversationId, bool pinned)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE conversations SET pinned = ? WHERE id = ?");
    query.addBindValue(pinned ? 1 : 0);
    query.addBindValue(conversationId);
    return query.exec();
}

bool DatabaseManager::setConversationUnread(const QString& conversationId, int unreadCount)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE conversations SET unread_count = ? WHERE id = ?");
    query.addBindValue(unreadCount);
    query.addBindValue(conversationId);
    return query.exec();
}

bool DatabaseManager::hideConversation(const QString& conversationId, bool hidden)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE conversations SET hidden = ? WHERE id = ?");
    query.addBindValue(hidden ? 1 : 0);
    query.addBindValue(conversationId);
    return query.exec();
}

bool DatabaseManager::deleteConversationMessages(const QString& conversationId)
{
    QSqlQuery remove(m_db);
    remove.prepare("DELETE FROM messages WHERE conversation_id = ?");
    remove.addBindValue(conversationId);
    if (!remove.exec()) {
        return false;
    }

    QSqlQuery reset(m_db);
    reset.prepare("UPDATE conversations SET last_message = '', unread_count = 0 WHERE id = ?");
    reset.addBindValue(conversationId);
    return reset.exec();
}

bool DatabaseManager::execSql(const QString& sql) const
{
    QSqlQuery query(m_db);
    if (!query.exec(sql)) {
        qWarning() << "执行 SQL 失败:" << query.lastError().text() << sql;
        return false;
    }
    return true;
}
