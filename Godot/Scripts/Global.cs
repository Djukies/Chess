using Godot;
using System;

public partial class Global : Node
{
	public static Global Instance { get; private set; }
	
	public override void _Ready()
	{
		Instance = this;
	}
	public int poop = 3;
	
	public static int VectorToInt(Vector2I position)
	{
		return position.X + (position.Y * 8);
	}
	
	public static Vector2I IntToVector(int position)
	{
		return new Vector2I(position % 8, position / 8);
	}
}
