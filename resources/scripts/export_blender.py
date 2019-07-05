import bpy
import bmesh
import os
import sys
import struct

class CE_Exporter():
    args = sys.argv[sys.argv.index("--") + 1:]
    scene = bpy.context.scene
    timeline_markers = bpy.context.scene.timeline_markers
    obj = bpy.context.active_object
    arm = None
    
    frame_start = scene.frame_start
    frame_end = scene.frame_end
    frame_size = frame_end - frame_start + 1
    scale = 1.0
    
    fd = args[0]
    fn = args[1]
    
    frame_offset = 0

    class object_entry:
        def __init__(self, obj):
            self.obj = obj
            self.children = []
    
    def execute(self):
        print("---------export start----------")
        if os.access(self.fd, os.W_OK) is False:
            print("!permission denied : " + dirr)
            return False
        print ("!writing to: " + self.fd + self.fn + ".prefab")
        
        object_entries = []
        
        for obj in self.scene.objects:
            if obj.parent:
                self.add_child_entry(object_entries, obj, obj.parent.name)
            else:
                object_entries.append(self.object_entry(obj))
        
        prefab_file = open(self.fd + self.fn + ".prefab", "wb")
        
        self.write(prefab_file, "{\n")
        self.export_entries(prefab_file, object_entries, "  ")
        self.write(prefab_file, "}")
        
        print ("-------export end--------")

    def add_child_entry(self, object_entries, obj, pnm):
        for e in object_entries:
            if e.obj.name == pnm:
                e.children.append(self.object_entry(obj))
                return True
            if self.add_child_entry(e.children, obj, pnm):
                return True
        return False
    
    def export_entries(self, prefab_file, entries, indent):
        indent2 = indent + 2 * " "
        n = len(entries)
        for i, e in enumerate(entries, 1):
            print ("object " + e.obj.name)
            self.write(prefab_file, indent + '"object":{\n')
            self.write(prefab_file, indent2 + '"name":"' + e.obj.name + '",\n')
            self.write(prefab_file, indent2 + '"position":[],\n')
            self.write(prefab_file, indent2 + '"rotation":[],\n')
            self.write(prefab_file, indent2 + '"scale":[],\n')

            if len(e.children) > 0:
                self.write(prefab_file, indent2 + '"children":{\n')
                self.export_entries(prefab_file, e.children, indent + 4 * " ")
                self.write(prefab_file, indent2 + '}\n')
            
            if i < n:
                self.write(prefab_file, indent + '},\n')
            else:
                self.write(prefab_file, indent + '}\n')

    def write(self, file, str):
        file.write(str.encode())

if __name__ == "__main__":
    CE_Exporter().execute()
