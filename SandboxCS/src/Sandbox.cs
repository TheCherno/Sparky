using System;

using SparkyCLI;

public class Sandbox
{
    public static void Main(string[] args)
    {
        Window window = new Window("Test", 960, 540);
        while (!window.Closed())
        {
            Vector2 mouse = window.GetMousePosition();
            Console.WriteLine("{0}, {1}", mouse.x, mouse.y);
            window.Update();
        }
    }
}