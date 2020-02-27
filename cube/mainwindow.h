#pragma once

#include <QMainWindow>
#include <QMessageBox>
#include <vector>
#include "cube.h"
#include "square.h"
#include "enums.h"
#include "colorcalibrator.h"
#include "colorreader.h"
#include "arduino.h"
#include "layerssolver.h"
#include "optimalsolver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public Observer<Solver>
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setupSquares();
    void setRotationsNumber(int rotationsNumber);
    void changeColor(QPushButton *square);
    void showDialog(std::string message);
    void onUpdate(Solver& source, int number);

    int rotationsNumber;
    Mode mode = Mode::SIMULATION;
    std::shared_ptr<Cube> cube;
    std::shared_ptr<Microcontroller> microcontroller;
    std::shared_ptr<Rotator> rotator;
    std::shared_ptr<Solver> layersSolver;
    std::shared_ptr<Solver> optimalSolver;
    std::unique_ptr<ColorCalibrator> calibrator;
    std::unique_ptr<ColorReader> reader;
    std::map<std::string, Square> squares;

    // Will be excluded
    void solve(std::string s);

private slots:
    void on_randomButton_clicked();
    void on_restartButton_clicked();
    void on_toggleModeButton_clicked();

    void on_crossButton_clicked();
    void on_firstLayerCornersButton_clicked();
    void on_secondLayerButton_clicked();
    void on_downCrossButton_clicked();
    void on_downEdgesButton_clicked();
    void on_placeDownCornersButton_clicked();
    void on_turnDownCornersButton_clicked();

    void on_solveOptimalButton_clicked();
    void on_solveLanesButton_clicked();
    void on_colorCalibrationButton_clicked();
    void on_readColorsButton_clicked();

    void on_rotationUButton_clicked();
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

    void on_up9Button_clicked();
    void on_front3Button_clicked();
    void on_right1Button_clicked();
    void on_left3Button_clicked();
    void on_front1Button_clicked();
    void on_up7Button_clicked();
    void on_up3Button_clicked();
    void on_right3Button_clicked();
    void on_back9Button_clicked();
    void on_up1Button_clicked();
    void on_left1Button_clicked();
    void on_back7Button_clicked();
    void on_front9Button_clicked();
    void on_right7Button_clicked();
    void on_down3Button_clicked();
    void on_front7Button_clicked();
    void on_left9Button_clicked();
    void on_down1Button_clicked();
    void on_down9Button_clicked();
    void on_right9Button_clicked();
    void on_back3Button_clicked();
    void on_back1Button_clicked();
    void on_down7Button_clicked();
    void on_left7Button_clicked();
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
private:
    Ui::MainWindow *ui;
};
