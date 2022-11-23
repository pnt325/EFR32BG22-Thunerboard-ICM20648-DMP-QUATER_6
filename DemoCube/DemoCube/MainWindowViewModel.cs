using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DemoCube
{
    public class MainWindowViewModel : ViewModelBase
    {
        private double angleX;
        private double angleY;
        private double angleZ;

        public double AngleX { get => angleX; set => SetProperty(ref angleX,value); }
        public double AngleY { get => angleY; set => SetProperty(ref angleY, value); }
        public double AngleZ { get => angleZ; set => SetProperty(ref angleZ, value); }
    }
}
