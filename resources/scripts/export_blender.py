import bpy
import bmesh
import os
import sys
import struct
import shutil

class CE_Exporter():
    SIGNATURE = "ChokoEngine Mesh 20"

    args = sys.argv[sys.argv.index("--") + 1:]
    scene = bpy.context.scene
    timeline_markers = bpy.context.scene.timeline_markers
    
    frame_start = scene.frame_start
    frame_end = scene.frame_end
    frame_size = frame_end - frame_start + 1
    scale = 1.0
    
    rootfd = args[0]
    relfd = args[1]
    fd = rootfd + relfd
    fn = args[2]
    
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
            if obj.type == 'MESH' or obj.type == 'ARMATURE':
                if obj.parent:
                    self.add_child_entry(object_entries, obj, obj.parent.name)
                else:
                    object_entries.append(self.object_entry(obj))
        
        self.cleandir(self.fd + self.fn + ".blend/")

        prefab_file = open(self.fd + self.fn + ".prefab", "wb")
        self.write(prefab_file, '{\n  "object":{\n')
        indent2 = 4 * ' '
        self.write(prefab_file, indent2 + '"name":"' + self.fn + '",\n')
        self.write(prefab_file, indent2 + '"position":[ "0", "0", "0" ],\n')
        self.write(prefab_file, indent2 + '"rotation":[ "1", "0", "0", "0" ],\n')
        self.write(prefab_file, indent2 + '"scale":[ "1", "1", "1" ],\n')
        self.write(prefab_file, indent2 + '"children":{\n')
        
        self.export_entries(prefab_file, object_entries, 6 * ' ')

        self.write(prefab_file, indent2 + "}\n  }\n}")
        
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
        indent3 = indent2 + 2 * " "
        indent4 = indent3 + 2 * " "
        indent5 = indent4 + 2 * " "
        n = len(entries)
        for i, e in enumerate(entries, 1):
            print ("object " + e.obj.name)
            self.write(prefab_file, indent + '"object":{\n')
            self.write(prefab_file, indent2 + '"name":"' + e.obj.name + '",\n')
            if e.obj.parent_type == "BONE":
                self.write(prefab_file, indent2 + '"parent_bone":"' + e.obj.parent_bone + '",\n')
            poss = e.obj.location
            self.write(prefab_file, indent2 + '"position":[ "{:f}", "{:f}", "{:f}" ],\n'.format(poss[0], poss[2], poss[1]))
            rott = e.obj.rotation_quaternion
            self.write(prefab_file, indent2 + '"rotation":[ "{:f}", "{:f}", "{:f}", "{:f}" ],\n'.format(rott[0], rott[1], rott[2], rott[3]))
            scll = e.obj.scale
            self.write(prefab_file, indent2 + '"scale":[ "{:f}", "{:f}", "{:f}" ],\n'.format(scll[0], scll[2], scll[1]))

            if e.obj.type == 'MESH':
                self.write(prefab_file, indent2 + '"components":{\n' + indent3 + '"MeshRenderer":{\n')
                self.write(prefab_file, indent4 + '"mesh":"' + self.relfd + self.fn + '.blend/' + e.obj.name + '.mesh' + '",\n')
                self.write(prefab_file, indent4 + '"modifiers":{},\n')
                self.write(prefab_file, indent4 + '"materials":[\n')
                self.write(prefab_file, indent5 + '"def.material"\n')
                self.write(prefab_file, indent4 + ']\n')
                self.write(prefab_file, indent3 + '}\n')
                self.export_mesh(self.fd + self.fn + '.blend/' + e.obj.name + '.mesh', e.obj)
            elif e.obj.type == 'ARMATURE':
                self.write(prefab_file, indent2 + '"components":{\n' + indent3 + '"Rig":{\n')
                self.write(prefab_file, indent4 + '"armature":"' + self.relfd + self.fn + '.blend/' + e.obj.name + '.armature' + '"\n')
                self.write(prefab_file, indent3 + '}\n')
                self.export_armature(self.fd + self.fn + '.blend/' + e.obj.name + '.armature', e.obj)

            if len(e.children) > 0:
                self.write(prefab_file, indent2 + '},\n')
                self.write(prefab_file, indent2 + '"children":{\n')
                self.export_entries(prefab_file, e.children, indent3)
                self.write(prefab_file, indent2 + '}\n')
            else:
                self.write(prefab_file, indent2 + '}\n')
            if i < n:
                self.write(prefab_file, indent + '},\n')
            else:
                self.write(prefab_file, indent + '}\n')

    def export_mesh(self, path, obj):
        arma = None
        for mod in obj.modifiers:
            if mod.type == 'ARMATURE':
                arma = mod
                obj.modifiers.remove(mod)
                break
        
        #apply modifiers
        bm = bmesh.new()
        bm.from_mesh(obj.to_mesh(bpy.context.scene, False, 'PREVIEW'))
        bpy.context.scene.objects.active = obj
        if bm.verts.layers.shape.keys():
            obj.modifiers.clear()
        obj.modifiers.new("EdgeSplit", 'EDGE_SPLIT')
        obj.modifiers["EdgeSplit"].split_angle = 1.0472 #60 degrees
        obj.modifiers["EdgeSplit"].use_edge_sharp = True
        obj.modifiers.new("Triangulate", 'TRIANGULATE')
        m = obj.to_mesh(bpy.context.scene, True, 'PREVIEW')
        
        #extract and clean vertex data
        verts = m.vertices
        vrts = [] #(vert_id, uv)
        loop2vrt = [] #int
        vrt2vert = [] #int
        
        if m.uv_layers:
            uid = 0
            vrtc = 0
            for uvl in m.uv_layers[0].data:
                pr = (m.loops[uid].vertex_index, uvl.uv)
                try:
                    vid = vrts.index(pr)
                except ValueError:
                    vrts.append(pr)
                    loop2vrt.append(vrtc)
                    vrt2vert.append(pr[0])
                    vrtc = vrtc + 1
                else:
                    loop2vrt.append(vid)
                uid = uid + 1
        vrtsz = len(vrts)

        #write data
        print ("!writing to: " + path)
        file = open(path, "wb")
        self.write(file, self.SIGNATURE + "\x00V")

        #V size4 [(3xvert4 3xnorm4) array]
        file.write(struct.pack("<i", vrtsz))
        for vrt in vrts:
            v = verts[vrt[0]]
            file.write(struct.pack("<fff", v.co[0], v.co[2], v.co[1]))
            file.write(struct.pack("<fff", v.normal[0], v.normal[2], v.normal[1]))
        
        #F size4 [{material1 3xface4} array]
        self.write(file, "F")
        file.write(struct.pack("<i", len(m.polygons)))
        for poly in m.polygons:
            file.write(struct.pack("<B", poly.material_index))
            file.write(struct.pack("<iii", loop2vrt[poly.loop_indices[0]], loop2vrt[poly.loop_indices[2]], loop2vrt[poly.loop_indices[1]]))
        
        #U size1 [uv04 array] ([uv14 array])
        if m.uv_layers:
            self.write(file, "U")
            file.write(struct.pack("<B", 1))
            for v in vrts:
                file.write(struct.pack("<ff", v[1][0], v[1][1]))
        
        #G size1 [groupName NULL array] (for each vert)[{groupSz1 [groupId1 groupWeight4 array]} array]
        if len(obj.vertex_groups) > 0:
            self.write(file, "G")
            file.write(struct.pack("<B", len(obj.vertex_groups)))
            for grp in obj.vertex_groups:
                self.write(file, grp.name + "\x00")
            for vrt in vrts:
                vert = verts[vrt[0]]
                file.write(struct.pack("<B", len(vert.groups)))
                for grp in vert.groups:
                    file.write(struct.pack("<Bf", grp.group, grp.weight))
        
        #S size1 [name NULL [3xdelta4 array] array]
        if len(bm.verts.layers.shape.keys()) > 1:
            isFirst = True
            self.write(file, "S")
            file.write(struct.pack("<B", len(bm.verts.layers.shape.keys()) - 1))
            bm.verts.ensure_lookup_table()
            
            vert2bvt = []
            for v2 in verts:
                i = 0
                for v in bm.verts:
                    if v2.co == v.co:
                        vert2bvt.append(i)
                        break
                    i = i + 1
            
            for key in bm.verts.layers.shape.keys():
                if isFirst:
                    isFirst = False
                else:
                    val = bm.verts.layers.shape.get(key)
                    self.write(file, key + "\x00")
                    for vt in vrt2vert:
                        v = bm.verts[vert2bvt[vt]]
                        delta = v[val] - v.co
                        file.write(struct.pack("<fff", delta[0], delta[2], delta[1]))
            
        file.close()
    
    def export_armature(self, path, arm):
        bones = []
        for b in arm.data.bones:
            if not b.parent:
                bones.append(b)

        print ("!writing to: " + path)
        file = open(path, "wb")
        indent = 2 * ' '
        indent2 = indent + 2 * ' '
        self.write(file, '{\n' + indent + '"armature":{\n')
        nb = len(bones)
        for i, b in enumerate(bones, 1):
            self.export_bone(file, b, indent2, i == nb)
        self.write(file, indent + '}\n}')

    def export_bone(self, file, bone, indent, last):
        indent2 = indent + 2 * " "
        indent3 = indent2 + 2 * " "
        self.write(file, indent + '"' + bone.name + '":{\n')
        vec = bone.head
        self.write(file, indent2 + '"head":[ "{:f}", "{:f}", "{:f}" ],\n'.format(vec[0], vec[2], vec[1]))
        vec = bone.tail - vec
        self.write(file, indent2 + '"tail":[ "{:f}", "{:f}", "{:f}" ],\n'.format(vec[0], vec[2], vec[1]))
        vec = bone.z_axis
        self.write(file, indent2 + '"front":[ "{:f}", "{:f}", "{:f}" ],\n'.format(vec[0], vec[2], vec[1]))
        self.write(file, indent2 + '"connected":' + ('"1"' if bone.use_connect else '"0"'))
        nc = len(bone.children)
        if nc > 0:
            self.write(file, ',\n' + indent2 + '"children":{\n')
            for i, b in enumerate(bone.children, 1):
                self.export_bone(file, b, indent3, i == nc)
            self.write(file, indent2 + '}\n')
        else:
            self.write(file, '\n')
        self.write(file, indent + ('}\n' if last else '},\n'))

    def cleandir(self, path):
        if os.path.isdir(path):
            shutil.rmtree(path)
        os.mkdir(path)

    def write(self, file, str):
        file.write(str.encode())

if __name__ == "__main__":
    CE_Exporter().execute()
