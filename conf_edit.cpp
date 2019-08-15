#include "conf_edit.h"
#include "exec_cmd.h"
#include "file_uploading.h"
#include "ui_conf_edit.h"

#include <QDir>
#include <QMessageBox>
extern QString project_dir;
static QString base_xml_dir = "/mnt/share/3.project/iceberg/deploy.xml.d/";

conf_edit::conf_edit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::conf_edit)
{
    ui->setupUi(this);
    ui->logs->setText("Loading ......");
    id = startTimer(1000);
}

conf_edit::~conf_edit()
{
    delete ui;
}


void conf_edit::alert(const QString &str)
{
    QString str_q = str;
    QMessageBox *msg = new QMessageBox("warning",str_q,
                                       QMessageBox::Critical,QMessageBox::Ok | QMessageBox::Default,
                                       QMessageBox::Cancel | QMessageBox::Escape,0);
    msg->show();
}


void conf_edit::update_single_xml()
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

void conf_edit::update_service_name()
{
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/get_services.sh " + project_dir;
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

QString conf_edit::deply_xml_path()
{
   auto xml = base_xml_dir + ui->service->currentText() + "/"+ui->xml->currentText()+".xml";
   return xml;
}

void conf_edit::update_node()
{
    if(ui->service->currentText() == "" || ui->xml->currentText() == "")
    {
        return ;
    }
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/get_service_node.sh " + project_dir +
            " " + deply_xml_path() + " "  + ui->service->currentText();
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    json_obj ips;
    try
    {
        ips.loads(output.c_str());
        ui->node_ip->clear();
        for (auto node_ip : ips.array_val)
        {
            ui->node_ip->addItem(QString(node_ip.s_val.c_str()));
        }
        update_files();
    }
    catch(...)
    {
        alert("update_node failed");
    }
}

void conf_edit::update_dir()
{
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/dirs.sh ";
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    json_obj all_dirs;
    try
    {
        all_dirs.loads(output.c_str());
        for (auto dir : all_dirs.array_val)
        {
            ui->dir->addItem(QString(dir.s_val.c_str()));
        }
        update_files();
    }
    catch(...)
    {
        alert("update_dir failed");
    }
}

void conf_edit::update_files()
{
    if(ui->node_ip->currentText() == "" || ui->dir->currentText() == "")
    {
        return;
    }
    ui->file->clear();
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/list_files.sh " + project_dir
            + " " + ui->node_ip->currentText() + " " + ui->dir->currentText();
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    json_obj all_files;
    try
    {
        all_files.loads(output.c_str());
        for (auto file : all_files.array_val)
        {
            ui->file->addItem(QString(file.s_val.c_str()));
        }
    }
    catch(...)
    {
        alert("update_dir failed");
    }
}

void conf_edit::update_file()
{
    if(ui->node_ip->currentText() == "" || ui->dir->currentText() == "" ||  ui->file->currentText() == "")
    {
        return;
    }
    ui->logs->clear();
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/cat_file.sh " + project_dir
            + " " + ui->node_ip->currentText() + " " + ui->dir->currentText() + " " + ui->file->currentText();
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err) return;
    ui->logs->setText(QString(output.c_str()));
    update_conf_color();
}

void conf_edit::timerEvent(QTimerEvent *event)
{
    update_service_name();
    update_single_xml();
    update_dir();
    update_node();
    killTimer(id);
}

void conf_edit::update_color_key_color(QTextEdit *item, const QString &key, const QBrush &brush)
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

void conf_edit::on_service_currentTextChanged(const QString &arg1)
{
    update_node();
}

void conf_edit::on_xml_currentTextChanged(const QString &arg1)
{
    update_node();
}

void conf_edit::on_dir_currentTextChanged(const QString &arg1)
{
    update_files();
}

void conf_edit::on_file_currentTextChanged(const QString &arg1)
{
    update_file();
}

void conf_edit::update_conf_color()
{
    ui->logs->setTextColor(Qt::white);
    update_color_key_color(ui->logs,QString("*=*"),Qt::blue);
    update_color_key_color(ui->logs,QString("#*"),Qt::gray);
}

std::string local_tmp_conf = "local_tmp_conf.file";
void conf_edit::on_save_clicked()
{
    QString str =  ui->logs->toPlainText();
    int err = write_str2file(local_tmp_conf.c_str(), str.toStdString().c_str());
    if(err)
    {
        alert("write_str2file failed");
    }
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/copy_file.sh " + project_dir
            + " " + ui->node_ip->currentText() + " " + ui->dir->currentText()
            + " " + ui->file->currentText() + " " +basedir + "/" + QString(local_tmp_conf.c_str());

    json_obj after_cmds;
    auto tmp_cmd = basedir +"/../release_tool/reload_conf.sh " + project_dir
            + " " + ui->node_ip->currentText() + " " + ui->service->currentText();
    after_cmds.append(json_obj(tmp_cmd.toStdString()));

    tmp_cmd = basedir +"/../release_tool/get_conf.sh " + project_dir
                + " " + ui->node_ip->currentText() + " " + ui->service->currentText();
    after_cmds.append(json_obj(tmp_cmd.toStdString()));

    exec_cmd * dlg = new exec_cmd("upload file", cmd, after_cmds);

    dlg->exec();
    delete dlg;
}
