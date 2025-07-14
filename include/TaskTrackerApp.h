#ifndef TASKTRACKERAPP_H
#define TASKTRACKERAPP_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QLabel>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QCheckBox>
#include "Task.h"
#include "TaskWidget.h"


class TaskTrackerApp : public QWidget{
    Q_OBJECT

    public:
        explicit TaskTrackerApp(QWidget* parent = nullptr);

    private slots:
        void addTask();
        void removeTask(const Task& task);
        void toggleCompleteTask(const Task& task);
        void editTask(const Task& task);
        void exportTasks();
        void importTasks();
    private:
        QStringList savedCategories;

        //Task Input UI
        QLineEdit* titleInput;
        QDateEdit* dueDateInput;
        QComboBox* priorityInput;
        QPushButton* addButton;

        //Layouts
        QVBoxLayout* taskListLayout;
        QWidget* taskListContainer;
        QScrollArea* scrollArea;

        //Task Storage
        QList<Task> tasks;

        //Filtering
        QComboBox* filterComboBox;
        QComboBox* sortComboBox;
        QList<Task> getFilteredTasks() const;

        //Notifications
        QTimer* reminderTimer;
        QSystemTrayIcon* trayIcon;

        //SearchBar
        QLineEdit* searchInput;

        //Category
        QLineEdit* categoryInput;
        QComboBox* categoryFilterComboBox;

        void checkReminders();
        void sortTasks(QList<Task>& list) const;
        void refreshTaskList();
        void saveTasks() const;
        void loadTasks();
};

#endif