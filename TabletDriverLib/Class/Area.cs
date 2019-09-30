namespace TabletDriverLib.Class
{
    public class Area
    {
        public Area()
        {
        }

        public Area(double width, double height, Vector pos, double rotation = 0)
        {
            Width = width;
            Height = height;
            Position = pos;
            Rotation = rotation;
        }

        public double Width { set; get; } = 0;
        public double Height { set; get; } = 0;
        public Vector Position { set; get; } = Vector(0, 0);
        public double Rotation { set; get; } = 0;
    }
}