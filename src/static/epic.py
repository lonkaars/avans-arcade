#!/bin/python3
import re

# Read in the first file and extract the id and image filename for each tile
tile_info = {}
with open('dynamic.tsx', 'r') as f:
    for line in f:
        id_match = re.match(r'<tile id="(\d+)">', line.strip())
        if id_match:
            tile_id = int(id_match.group(1))
        else:
            image_match = re.search(r'source=".*?/(\w+_\d+\.png)"', line.strip())
            if image_match:
                image_filename = image_match.group(1)
                tile_info[tile_id] = image_filename

# Read in the second file and extract the size for each tile
tile_sizes = {}
with open('tilemap.h', 'r') as f:
    for line in f:
        size_match = re.match(r'#define HH_TM_(\w+)_OFFSET (\d+)', line.strip(), re.IGNORECASE)
        if size_match:
            tile_type = size_match.group(1).lower()
            size = int(size_match.group(2))
            tile_sizes[tile_type] = size

# Read in the third file, update the values, and write to the output file
with open('map.csv', 'r') as input_file, open('out.csv', 'w') as output_file:
    for line in input_file:
# #OK
        tile_ids = [int(tile_id) for tile_id in line.strip().split(',')]
        updated_tile_ids = []
        for tile_id in tile_ids:
            image_filename = re.match(r'(\w+)_(\d+)\.png', tile_info[tile_id])
            tile_type = image_filename.group(1).lower()
            offset = int(image_filename.group(2))
            size = tile_sizes[tile_type]
            updated_tile_id = offset + size
            updated_tile_ids.append(str(updated_tile_id))
        output_file.write(','.join(updated_tile_ids) + '\n')

