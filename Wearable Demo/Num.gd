extends Label


# Declare member variables here. Examples:
# var a = 2
# var b = "text"
var time = 0.0
var val = 0

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	time += delta
	text = str("%0.1f" % time)

func _on_Area2D_body_entered(body):
	if(val == 1):
		if(Yucky.true_time == 0.0):
			Yucky.true_time = time
		elif(Yucky.true_time > time):
			Yucky.true_time = time
		print(Yucky.true_time)
	time = 0.0
	val += 1
