/**
 FFT From Image. Apply to a microscopy image, returning a I-q data set,
 allowing comparisson with Small Angle X-ray Scattering experiments.
 Copyright © 2015 Pablo Hernandez-Cerdan

 This library is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this library; if not, see <http://www.gnu.org/licenses/>.
*/

#ifndef RADIALTABWIDGET_H_
#define RADIALTABWIDGET_H_
#include "radial_intensity.h"

#include "QVTKWidget.h"
#include <QFutureWatcher>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

// ITK
#include "image_functions.h"
#include <itkImageToVTKImageFilter.h>
// VTK
#include <vtkContextView.h>
#include <vtkSmartPointer.h>
class RadialTabWidget : public QMainWindow {
    Q_OBJECT

 public:
    explicit RadialTabWidget(QWidget *parent = 0);
    virtual ~RadialTabWidget();
    // Store images with double precission.
    using PixelType = double;
    using Image2DType = itk::Image<PixelType, 2>;
    using Image2DPointer = typename Image2DType::Pointer;
    using ComplexImage2DType =
        typename itk::ForwardFFTImageFilter<Image2DType>::OutputImageType;
    using ComplexImage2DPointer = typename ComplexImage2DType::Pointer;

    using VTKConnector2DType = itk::ImageToVTKImageFilter<Image2DType>;
    using VTKConnector2DPointer = typename VTKConnector2DType::Pointer;
    /**
     * Save parameters from dialog.
     *
     * @param input_filename
     * @param saveToFile
     * @param output_filename
     */
    void StoreInputParameters(
        const std::string & input_filename,
        bool saveToFile,
        const std::string & output_filename);
    /** Populate the widget (called from main window)
     * pre: Needs StoreInputParameters
     */
    void Populate();
  signals:
    void finished_ComputeRadialIntensity();
  public slots:
    void SetInput2D(std::string input_file);
    void SetFFT2D();
    void ComputeRadialIntenstiyFromFFT();
    void SetRadialPlot2D();
    void SaveRadialPlot2D(const std::string &fname);

  private:
    // Parameters from new dialog
    std::string input_filename;
    bool saveToFile;
    std::string output_filename;

    // Concurrent
    QFutureWatcher<void> fftWatcher;
    // ITK/VTK data
    Image2DPointer inputImage2D;
    ComplexImage2DPointer shiftedFFT2D;
    Image2DPointer magnitudeShiftedFFT2D;
    Image2DPointer magnitudeShiftedFFT2DLogWindowed;
    radial_intensity::IntensitiesHistoPair intensities_histo;
    radial_intensity::FlattenIntensities average_radial_intensities;
    vtkSmartPointer<vtkContextView> plotView;

    QDockWidget *dockWidgetInput;
    QDockWidget *dockWidgetFFT;
    QDockWidget *dockWidgetPlot;
    QVTKWidget *qvtkWidgetInput;
    QVTKWidget *qvtkWidgetFFT;
    QVTKWidget *qvtkWidgetPlot;
};
#endif
