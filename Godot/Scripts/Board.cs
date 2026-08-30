using Godot;
using System;
using Godot.Collections;

public partial class Board : Node2D
{
    private MoveGeneration moveGenerator;
    private TileMapLayer movesTiles;

    public override void _Ready()
    {
        base._Ready();
        moveGenerator = GetNode<MoveGeneration>("MoveGeneration");
        movesTiles = GetNode<TileMapLayer>("Moves");
    }

    private void OnPieceClicked(Vector2I position)
    {
        GD.Print(position);
        Array<int> moves = moveGenerator.GenerateMoves();
        GD.Print(moves);
        GD.Print(Global.IntToVector(moves[0]));
        foreach (var move in moves)
        {
            Vector2I movePos = Global.IntToVector(move);
            movesTiles.SetCell(movePos, 0, new Vector2I(0, 0));
        }
    }
}
