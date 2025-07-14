#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QUuid>
#include <QDate>

class Task{
    public:
        Task(
            const QString& title = "",
            const QString& description = "",
            const QDate& dueDate = QDate::currentDate(),
            const QString& category = "",
            int priority = 1,
            bool completed = false,
            const QString& id = QUuid::createUuid().toString()
        );

        QString getId() const { return id; }
        QString getTitle() const;
        QString getDescription() const;
        QDate getDueDate() const;
        QString getCategory() const;

        int getPriority() const;
        bool isCompleted() const;
        bool isNotified() const { return notified; }

        void setId(const QString& newId){ id = newId; }
        void setTitle(const QString& title);
        void setDescription(const QString& desc);
        void setDueDate(const QDate& date);
        void setCategory(const QString& cat);
        void setPriority(int p);
        void setCompleted(bool c);
        void setNotified(bool value) { notified = value; }

        QJsonObject toJson() const;
        static Task fromJson(const QJsonObject& obj);
    private:        
        QString id;
        QString title;
        QString description;
        QDate dueDate;
        QString category;
        int priority;
        bool completed;
        bool notified = false;
};

#endif