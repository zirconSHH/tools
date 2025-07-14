import json

source_json = "A:\\代码\\tools\\openpose_json_processor\\pose.json"
target_json = "A:\\代码\\tools\\openpose_json_processor\\result.json"

def flip_character(keypoints, canvas):
	points = []
	for j in range(0, len(keypoints), 3):
		x = keypoints[j]
		y = keypoints[j+1]
		conf = keypoints[j+2]
		points.extend([canvas - x, y, conf])

	return points

def center_character(keypoints, canvas):
	left = canvas
	right = 0
	for i in range(0, len(keypoints), 3):
		if keypoints[i+2]:
			if keypoints[i] > right:
				right = keypoints[i]
			elif keypoints[i] < left:
				left = keypoints[i]

	offset = canvas/2 - (right+left)/2

	points = []
	for j in range(0, len(keypoints), 3):
		x = keypoints[j]
		y = keypoints[j+1]
		conf = keypoints[j+2]
		points.extend([x + offset, y, conf])

	return points

def main():
	with open(source_json) as f1:
		json_obj = json.load(f1)

	# 提取画布尺寸
	canvas_width = json_obj["canvas_width"]
	canvas_height = json_obj["canvas_height"]

	if json_obj["people"]:
		original_person = json_obj["people"][0]
		original_keypoints = original_person["pose_keypoints_2d"]
		# flipped_keypoints = flip_character(original_keypoints, canvas_width)
		# new_character = {"pose_keypoints_2d": flipped_keypoints}
		centered_keypoints = center_character(original_keypoints, canvas_width)
		new_character = {"pose_keypoints_2d": centered_keypoints}
		json_obj["people"] = []
		json_obj["people"].append(new_character)

	with open(target_json, "w") as f2:
		json.dump(json_obj, f2, indent=2)

if __name__ == "__main__":
	main()