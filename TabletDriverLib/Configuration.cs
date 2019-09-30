namespace TabletDriverLib
{
    public class Configuration
    {
        public Configuration()
        {
        }

        public List<Area> DesktopAreas { set; get; } = new List<Area>();
        public List<Area> TabletAreas { set; get; } = new List<Area>();
    }
}