using Godot;
using System;

public partial class Pieces : TileMapLayer
{
	[Signal]
	public delegate void PieceClickedEventHandler(Vector2I piecePosition);
	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
	}

	// Called every frame. 'delta' is the elapsed time since the previous frame.
	public override void _Process(double delta)
	{
	}


	public override void _Input(InputEvent @event)
	{
		base._Input(@event);
		if (@event is InputEventMouseButton eventMouseButton)
		{
			if (eventMouseButton.Pressed)
			{
				if (eventMouseButton.ButtonIndex == MouseButton.Left)
				{
					var pos = LocalToMap(eventMouseButton.Position);
					EmitSignalPieceClicked(pos);
				}
			}
		}
	}
}
