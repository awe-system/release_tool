#include "deploy_op.h"
#include "ui_deploy_op.h"

#include <lt_data/json_obj.h>
#include <lt_function/utils.h>
#include <QDir>
#include <QMessageBox>
extern QString project_dir;
static QString base_xml_dir = "/mnt/share/3.project/iceberg/deploy.xml.d/";

QString deploy_op::md5_log()
{
    auto cmd = basedir +"/../release_tool/md5log.sh " + project_dir + " " + ui->dtag->text() ;
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        return QString("");
    }
    else
    {
        return QString(output.c_str());
    }
}

void deploy_op::update_logs()
{
    auto cmd = basedir +"/../release_tool/updatelog.sh " + project_dir + " " + ui->dtag->text() ;
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        alert(QString(output.c_str()));
    }
    else
    {
        ui->logs->setText(QString(output.c_str()));
        QTextCursor cursor = ui->logs->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->logs->setTextCursor(cursor);
        update_log_color();
    }
}

void deploy_op::timerEvent(QTimerEvent *event)
{
    QString cur_md5 = md5_log();
    if(last_md5 == cur_md5)
    {
        same_cnt++;
        if(same_cnt == 120)
        {
            killTimer(id);
            ui->deploy->setEnabled(true);
        }
    }
    else
    {
        same_cnt = 0;
        update_logs();
        last_md5 = cur_md5;
    }
}

deploy_op::deploy_op(const QString &commit,QWidget *parent) :
    QDialog(parent),same_cnt(0),
    ui(new Ui::deploy_op)
{
    basedir = QDir::currentPath();
    ui->setupUi(this);
    ui->commit->setText(commit);
    //FIXME
    add_mainversions();
    update_tags_by_by_mainversions();
//    add_tags();
//    update_tag_by_commit();
    update_commit_by_tag();
    ui->whole->setChecked(true);
    update_service_name();
    update_view_after_radio_select();
}

deploy_op::~deploy_op()
{
    delete ui;
}


void deploy_op::alert(const QString &str)
{
    QString str_q = str;
    QMessageBox *msg = new QMessageBox("warning",str_q,
                                       QMessageBox::Critical,QMessageBox::Ok | QMessageBox::Default,
                                       QMessageBox::Cancel | QMessageBox::Escape,0);
    msg->show();
}


//void deploy_op::add_tags()
//{
//    auto basedir = QDir::currentPath();
//    auto cmd = basedir +"/../release_tool/tags.sh " + project_dir;
//    std::string output;
//    auto err = buf_exec_cpp(cmd.toStdString(),output);
//    json_obj all_tags;
//    try
//    {
//        all_tags.loads(output.c_str());
//        for ( auto tag = all_tags.array_val.rbegin();
//              tag != all_tags.array_val.rend(); ++tag)
//        {
//            ui->tag->addItem(QString(tag->s_val.c_str()));
//        }
//    }
//    catch(...)
//    {
//        alert("there is no tag now");
//    }
//}

void deploy_op::add_mainversions()
{
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/tags.sh " + project_dir;
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    try
    {
        version_map.loads(output.c_str());
        for ( auto tag = version_map.dic_val.rbegin();
              tag != version_map.dic_val.rend(); ++tag)
        {
            ui->main_version->addItem(QString(tag->first.s_val.c_str()));
        }
    }
    catch(...)
    {
        alert("there is no tag now");
    }
}

void deploy_op::update_tags_by_by_mainversions()
{
    ui->tag->clear();
    try
   {
       json_obj all_tags = version_map[ui->main_version->currentText().toStdString()];
        for ( auto tag = all_tags.array_val.rbegin();
        tag != all_tags.array_val.rend(); ++tag)
        {
            ui->tag->addItem(QString(tag->s_val.c_str()));
        }
    }
    catch(...)
    {
        alert("there is no tag now");
    }
}

void deploy_op::update_tag_by_commit()
{
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/get_tag.sh " + project_dir + " " + ui->commit->text();
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    ui->tag->setCurrentText(QString(output.c_str()));
}

void deploy_op::update_service_name()
{
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/get_services.sh " + project_dir + " " + ui->commit->text();
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    json_obj all_services;
    try
    {
        all_services.loads(output.c_str());
        for (auto service : all_services.array_val)
        {
            ui->service->addItem(QString(service.s_val.c_str()));
        }
    }
    catch(...)
    {
        alert("there is no tag now");
    }
}

void deploy_op::update_commit_by_tag()
{
    checkout_tag();
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/git_commits.sh " + project_dir + " " + ui->tag->currentText();
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        alert(QString(output.c_str()));
        return ;
    }
    json_obj all_commits;
    try
    {
        all_commits.loads(output.c_str());
        if( all_commits.array_val.size() < 1)
        {
            throw 1;
        }
        ui->commit->setText(QString(all_commits.array_val[0].s_val.c_str()));
    }
    catch(...)
    {
        alert("there is no commit now");
    }
}

void deploy_op::checkout_tag()
{
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/checkout.sh " + project_dir + " " + ui->tag->currentText();
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        alert(QString(output.c_str()));
    }
}

void deploy_op::update_view_after_radio_select()
{
    if(ui->single->isChecked())
    {
        ui->service_layout->setVisible(true);
        update_single_xml();
    }
    else
    {
        ui->service_layout->setVisible(false);
        update_whole_xml();
    }
}

void deploy_op::update_whole_xml()
{
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/get_whole_xml.sh " + project_dir;
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        alert(QString(output.c_str()));
        return ;
    }
    json_obj all_xmls;
    try
    {
        ui->xml->clear();
        all_xmls.loads(output.c_str());
        for ( auto xml = all_xmls.array_val.begin();
              xml != all_xmls.array_val.end(); ++xml)
        {
            ui->xml->addItem(QString(xml->s_val.c_str()));
        }
    }
    catch(...)
    {
        alert("there is no xml now");
    }
}

void deploy_op::update_single_xml()
{
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/get_service_xml.sh " + project_dir + " " + ui->service->currentText();
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        alert(QString(output.c_str()));
        return ;
    }
    json_obj all_xmls;
    try
    {
        ui->xml->clear();
        all_xmls.loads(output.c_str());
        for ( auto xml = all_xmls.array_val.rbegin();
              xml != all_xmls.array_val.rend(); ++xml)
        {
            ui->xml->addItem(QString(xml->s_val.c_str()));
        }
    }
    catch(...)
    {
        alert("there is no xml now");
    }
}

void deploy_op::update_color_key_color(QTextEdit *item, const QString &key, const QBrush &brush)
{
    QString searchString = key;
    QTextDocument *document = item->document();

    if (searchString.isEmpty()) {
        return;
    }
    QTextCursor highlightCursor(document);
    QTextCursor cursor(document);

    cursor.beginEditBlock();

    QTextCharFormat plainFormat(highlightCursor.charFormat());
    QTextCharFormat colorFormat = plainFormat;
    colorFormat.setForeground(brush);

    while (!highlightCursor.isNull() && !highlightCursor.atEnd()) {
        highlightCursor = document->find(searchString, highlightCursor,
                                         QTextDocument::FindWholeWords);

        if (!highlightCursor.isNull()) {
            highlightCursor.movePosition(QTextCursor::WordRight,
                                         QTextCursor::KeepAnchor);
            highlightCursor.mergeCharFormat(colorFormat);
        }
    }
    cursor.endEditBlock();
}

void deploy_op::update_xml_content()
{
    auto basedir = QDir::currentPath();
    auto cmd = "cat " + base_xml_dir + deply_xml_path() + ".xml";

    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    ui->logs->setText(QString(output.c_str()));
    update_xml_color();
}

void deploy_op::update_xml_color()
{
    ui->logs->setTextColor(Qt::white);
    update_color_key_color(ui->logs,QString("<deploy>"),Qt::blue);
    update_color_key_color(ui->logs,QString("</deploy>"),Qt::blue);
    update_color_key_color(ui->logs,QString("/>"),Qt::blue);
    update_color_key_color(ui->logs,QString("<node"),Qt::blue);
    update_color_key_color(ui->logs,QString("<nodes>"),Qt::blue);
    update_color_key_color(ui->logs,QString("</nodes>"),Qt::blue);
    update_color_key_color(ui->logs,QString("<service"),Qt::blue);
    update_color_key_color(ui->logs,QString("name"),Qt::green);

    update_color_key_color(ui->logs,QString("ip"),Qt::green);
}

void deploy_op::update_log_color()
{
    ui->logs->setTextColor(Qt::white);
    update_color_key_color(ui->logs,QString("start"),Qt::green);
    update_color_key_color(ui->logs,QString("end"),Qt::green);
    update_color_key_color(ui->logs,QString("ok"),Qt::green);
    update_color_key_color(ui->logs,QString("failed"),Qt::red);
    update_color_key_color(ui->logs,QString("not"),Qt::yellow);
    update_color_key_color(ui->logs,QString("install"),Qt::yellow);
    update_color_key_color(ui->logs,QString("172.30.41.8:5000"),Qt::cyan);
}

void deploy_op::on_tag_currentTextChanged(const QString &arg1)
{
    update_commit_by_tag();
}

void deploy_op::on_single_clicked()
{
    update_view_after_radio_select();
}

void deploy_op::on_whole_clicked()
{
     update_view_after_radio_select();
}

void deploy_op::on_service_currentTextChanged(const QString &arg1)
{
    update_view_after_radio_select();
    update_xml_content();
}

void deploy_op::on_xml_currentTextChanged(const QString &arg1)
{
    update_xml_content();
}


QString deploy_op::deply_xml_path()
{
    QString xml;
    if(ui->single->isChecked())
    {
        xml = ui->service->currentText() + "/"+ui->xml->currentText();
    }
    else
    {
        xml = ui->xml->currentText();
    }
    return xml;
}

void deploy_op::on_deploy_clicked()
{
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/deploy.sh " + project_dir + " " + deply_xml_path();
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        alert("deploy wrong:" + QString(output.c_str()));
    }
    else
    {
        ui->dtag->setText(QString(output.c_str()));
        start_update_data();
    }
}

void deploy_op::start_update_data()
{
    ui->deploy->setEnabled(false);
    id = startTimer(1000);
}

void deploy_op::on_main_version_currentTextChanged(const QString &arg1)
{
    update_tags_by_by_mainversions();
}
