/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include <complex>
#include <string>

#include "itkComplexToImaginaryImageFilter.h"
#include "itkComplexToRealImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkForward1DFFTImageFilter.h"
#include "itkVnlForward1DFFTImageFilter.h"
#if defined(ITK_USE_FFTWD) || defined(ITK_USE_FFTWF)
#  include "itkFFTWForward1DFFTImageFilter.h"
#endif
#if defined(ITKUltrasound_USE_clFFT)
#  include "itkOpenCLForward1DFFTImageFilter.h"
#endif

template <typename FFTType>
int
doTest(const char * inputImage, const char * outputImagePrefix)
{
  using ImageType = typename FFTType::InputImageType;
  using ComplexImageType = typename FFTType::OutputImageType;

  using ReaderType = itk::ImageFileReader<ImageType>;
  using RealFilterType = itk::ComplexToRealImageFilter<ComplexImageType, ImageType>;
  using ImaginaryFilterType = itk::ComplexToImaginaryImageFilter<ComplexImageType, ImageType>;
  using WriterType = itk::ImageFileWriter<ImageType>;

  typename ReaderType::Pointer          reader = ReaderType::New();
  typename FFTType::Pointer             fft = FFTType::New();
  typename RealFilterType::Pointer      realFilter = RealFilterType::New();
  typename ImaginaryFilterType::Pointer imaginaryFilter = ImaginaryFilterType::New();
  typename WriterType::Pointer          writer = WriterType::New();

  reader->SetFileName(inputImage);
  fft->SetInput(reader->GetOutput());
  realFilter->SetInput(fft->GetOutput());
  imaginaryFilter->SetInput(fft->GetOutput());

  try
  {
    writer->SetInput(realFilter->GetOutput());
    writer->SetFileName(std::string(outputImagePrefix) + "Real.mha");
    writer->Update();

    writer->SetInput(imaginaryFilter->GetOutput());
    writer->SetFileName(std::string(outputImagePrefix) + "Imaginary.mha");
    writer->Update();
  }
  catch (itk::ExceptionObject & excep)
  {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    return EXIT_FAILURE;
  }

  fft.Print(std::cout);

  return EXIT_SUCCESS;
}

int
itkForward1DFFTImageFilterTest(int argc, char * argv[])
{
  if (argc < 3)
  {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage outputImagePrefix [backend]\n";
    std::cerr << "backend implementation options:\n";
    std::cerr << "  0 default\n";
    std::cerr << "  1 VNL\n";
    std::cerr << "  2 FFTW\n";
    std::cerr << "  3 OpenCL via clFFT\n";
    std::cerr << std::flush;
    return EXIT_FAILURE;
  }

  using PixelType = double;
  const unsigned int Dimension = 2;

  using ImageType = itk::Image<PixelType, Dimension>;
  using ComplexImageType = itk::Image<std::complex<PixelType>, Dimension>;

  int backend = 0;
  if (argc > 3)
  {
    backend = std::stoi(argv[3]);
  }

  if (backend == 0)
  {
    using FFTForwardType = itk::Forward1DFFTImageFilter<ImageType, ComplexImageType>;
    return doTest<FFTForwardType>(argv[1], argv[2]);
  }
  else if (backend == 1)
  {
    using FFTForwardType = itk::VnlForward1DFFTImageFilter<ImageType, ComplexImageType>;
    return doTest<FFTForwardType>(argv[1], argv[2]);
  }
  else if (backend == 2)
  {
#if defined(ITK_USE_FFTWD) || defined(ITK_USE_FFTWF)
    using FFTForwardType = itk::FFTWForward1DFFTImageFilter<ImageType, ComplexImageType>;
    return doTest<FFTForwardType>(argv[1], argv[2]);
#endif
  }
  else if (backend == 3)
  {
#if defined(ITKUltrasound_USE_clFFT)
    using FFTForwardType = itk::OpenCLForward1DFFTImageFilter<ImageType, ComplexImageType>;
    return doTest<FFTForwardType>(argv[1], argv[2]);
#endif
  }

  std::cerr << "Backend " << backend << " (" << argv[3] << ") not implemented" << std::endl;
  return EXIT_FAILURE;
}
