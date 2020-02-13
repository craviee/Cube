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
    void on_randomButton_clicked();

    void on_rotationUButton_clicked();

    void completeCube();

    void on_rotationUAButton_clicked();

    void on_rotationDButton_clicked();

    void on_rotationDAButton_clicked();

    void on_rotationRButton_clicked();

    void on_rotationRAButton_clicked();

    void on_rotationLButton_clicked();

    void on_rotationLAButton_clicked();

    void on_rotationFButton_clicked();

    void on_rotationFAButton_clicked();

    void on_rotationBButton_clicked();

    void on_rotationBAButton_clicked();

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

    void on_crossButton_clicked();

    void CruzSul();

    void CruzNorte();

    void CruzOeste();

    void CruzLeste();

    void on_restartButton_clicked();

    void QuinaSudeste();

    void on_up9Button_clicked();

    void on_front3Button_clicked();

    void on_right1Button_clicked();

    void QuinaSudoeste();

    void on_left3Button_clicked();

    void on_front1Button_clicked();

    void on_up7Button_clicked();

    void QuinaNordeste();

    void on_up3Button_clicked();

    void on_right3Button_clicked();

    void on_back9Button_clicked();

    void on_up1Button_clicked();

    void on_left1Button_clicked();

    void on_back7Button_clicked();

    void QuinaNoroeste();

    void SudesteBaixo();

    void on_front9Button_clicked();

    void on_right7Button_clicked();

    void on_down3Button_clicked();

    void SudoesteBaixo();

    void on_front7Button_clicked();

    void on_left9Button_clicked();

    void on_down1Button_clicked();

    void NordesteBaixo();

    void on_down9Button_clicked();

    void on_right9Button_clicked();

    void on_back3Button_clicked();

    void NoroesteBaixo();

    void on_back1Button_clicked();

    void on_down7Button_clicked();

    void on_left7Button_clicked();

    void on_firstLayerCornersButton_clicked();

    void frontSecond();

    void leftSecond();

    void rightSecond();

    void backSecond();

    void on_secondLayerButton_clicked();

    void debugaFront();

    void debugaLeft();

    void debugaRight();

    void debugaBack();

    void on_downCrossButton_clicked();

    void on_backEdgesButton_clicked();

    void on_placeDownEdgesButton_clicked();

    bool sudestecorreto();

    bool sudoestecorreto();

    bool nordestecorreto();

    bool noroestecorreto();

    void on_turnDownEdgesButton_clicked();

    void on_solveOptimalButton_clicked();

    QString color(QString cor);

    void solve(std::string s);

    void camera();

    void on_colorCalibrationButton_clicked();

    void on_readColorsButton_clicked();

    void setModel(std::vector<std::string> style, int c);

    bool verificaCoerencia();

    void on_back8Button_clicked();

    void on_back2Button_clicked();

    void on_back6Button_clicked();

    void on_back4Button_clicked();

    void on_down8Button_clicked();

    void on_down6Button_clicked();

    void on_down4Button_clicked();

    void on_down2Button_clicked();

    void on_front8Button_clicked();

    void on_front4Button_clicked();

    void on_front6Button_clicked();

    void on_front2Button_clicked();

    void on_left8Button_clicked();

    void on_left4Button_clicked();

    void on_left2Button_clicked();

    void on_left6Button_clicked();

    void on_right4Button_clicked();

    void on_right8Button_clicked();

    void on_right6Button_clicked();

    void on_right2Button_clicked();

    void on_up8Button_clicked();

    void on_up4Button_clicked();

    void on_up6Button_clicked();

    void on_up2Button_clicked();

    void takePictureAndProcess(std::vector<char> coresFaceAtual);

    void on_solveLanesButton_clicked();

    void on_toggleModeButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
