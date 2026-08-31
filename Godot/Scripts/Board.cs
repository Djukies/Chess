using Godot;
using Godot.Collections;
using Godot.NativeInterop;
using Array = System.Array;

public partial class Board : Node2D
{
    private Node moveGenerator;
    private TileMapLayer movesTiles;

    public override void _Ready()
    {
        base._Ready();
        moveGenerator = GetNodeOrNull<Node>("GDMoveGenerator");
        movesTiles = GetNodeOrNull<TileMapLayer>("Moves");
    }

    private void OnPieceClicked(Vector2I position)
    {
        GD.Print(position);
        int[] moves = moveGenerator.Call("generate_moves", Global.VectorToInt(position)).AsInt32Array();
        foreach (var move in moves)
        {
            Vector2I movePos = Global.IntToVector(move);
            movesTiles.SetCell(movePos, 0, new Vector2I(0, 0));
        }
    }
}
