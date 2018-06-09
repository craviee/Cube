#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
      QFile t;
    ~MainWindow();

private slots:
    void on_Randomize_clicked();

    void on_U_clicked();

    void completeCube();

    void on_UA_clicked();

    void on_D_clicked();

    void on_DA_clicked();

    void on_R_clicked();

    void on_RA_clicked();

    void on_L_clicked();

    void on_LA_clicked();

    void on_F_clicked();

    void on_FA_clicked();

    void on_B_clicked();

    void on_BA_clicked();

    void rotateU();

    void rotateUA();

    void rotateD();

    void rotateDA();

    void rotateR();

    void rotateRA();

    void rotateL();

    void rotateLA();

    void rotateF();

    void rotateFA();

    void rotateB();

    void rotateBA();

    void on_Cross_clicked();

    void CruzSul();

    void CruzNorte();

    void CruzOeste();

    void CruzLeste();

    void on_zerar_clicked();

    void on_CrossSul_clicked();

    void on_CrossNorte_clicked();

    void on_CruzLeste_clicked();

    void on_CruzOeste_clicked();

    void on_QuinaSudeste_clicked();

    void on_up9_clicked();

    void on_front3_clicked();

    void on_right1_clicked();

    void on_QuinaSudoeste_clicked();

    void on_left3_clicked();

    void on_front1_clicked();

    void on_up7_clicked();

    void on_QuinaNordeste_clicked();

    void on_up3_clicked();

    void on_right3_clicked();

    void on_back9_clicked();

    void on_up1_clicked();

    void on_left1_clicked();

    void on_back7_clicked();

    void on_QuinaNoroeste_clicked();

    void on_SudesteBaixo_clicked();

    void on_front9_clicked();

    void on_right7_clicked();

    void on_down3_clicked();

    void on_SudoesteBaixo_clicked();

    void on_front7_clicked();

    void on_left9_clicked();

    void on_down1_clicked();

    void on_NordesteBaixo_clicked();

    void on_down9_clicked();

    void on_right9_clicked();

    void on_back3_clicked();

    void on_NoroesteBaixo_clicked();

    void on_back1_clicked();

    void on_down7_clicked();

    void on_left7_clicked();

    void on_Quinas_clicked();

    void on_frontSecond_clicked();

    void on_leftSecond_clicked();

    void on_rightSecond_clicked();

    void on_backSecond_clicked();

    void on_segundaCamada_clicked();

    void on_consertaSegunda_clicked();

    void debugaFront();

    void debugaLeft();

    void debugaRight();

    void debugaBack();

    void on_Tudo_clicked();

    void on_CrossDown_clicked();

    void on_CantosDown_clicked();

    void on_Place_clicked();

    bool sudestecorreto();

    bool sudoestecorreto();

    bool nordestecorreto();

    bool noroestecorreto();

    void on_Turn_clicked();

    void on_otimoButton_clicked();

    QString color(QString cor);

    void solve(std::string s);

    void camera();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void setModel(std::vector<std::string> style, int c);

    bool verificaCoerencia();

    void on_back8_clicked();

    void on_back2_clicked();

    void on_back6_clicked();

    void on_back4_clicked();

    void on_down8_clicked();

    void on_down6_clicked();

    void on_down4_clicked();

    void on_down2_clicked();

    void on_front8_clicked();

    void on_front4_clicked();

    void on_front6_clicked();

    void on_front2_clicked();

    void on_left8_clicked();

    void on_left4_clicked();

    void on_left2_clicked();

    void on_left6_clicked();

    void on_right4_clicked();

    void on_right8_clicked();

    void on_right6_clicked();

    void on_right2_clicked();

    void on_up8_clicked();

    void on_up4_clicked();

    void on_up6_clicked();

    void on_up2_clicked();

    void calibrar();

    void takePictureAndProcess(std::vector<char> coresFaceAtual);

    void on_camadasReal_clicked();

    void on_otimoReal_clicked();

    void on_pushButton_3_clicked();

    void on_toggleMode_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
