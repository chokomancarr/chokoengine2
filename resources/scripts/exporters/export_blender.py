import bpy as bpy
import itertools
import os
import sys
import struct
import shutil
from mathutils import Quaternion

indent0 = ""
def indent(n):
    return indent0 + "  " * n
    
logfile = None
def printlog(msg):
    print(msg)
    logfile.write(msg + "\n")

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
    fd = rootfd + relfd #put everything in here
    fn = args[2]
    
    frame_offset = 0

    class object_entry:
        def __init__(self, obj):
            self.obj = obj
            self.children = []
            self.rig = None

    def execute(self):
        global logfile, indent0

        if bpy.app.version[1] < 80:
            printlog("fatal: please use Blender 2.8 or newer!!!")
            return

        if os.access(self.fd, os.W_OK) is False:
            print("!permission denied : " + self.fd)
            return False

        self.cleandir(self.fd)
            
        logfile = open(self.fd + self.fn + ".log.txt", "w")
        
        printlog("---------export start----------")
        printlog("!writing to: " + self.fd + self.fn + ".prefab")
        
        bpy.ops.object.mode_set(mode='OBJECT')
        #shared meshes break the exporter for now
        bpy.ops.object.make_single_user(type='ALL', object=True, obdata=True)
        
        #blender 2.8 does not arrange objects with dependancy
        arranged_objs = self.arrange_objs(self.scene.objects)
        
        object_entries = []
        
        has_arm = False
        
        for obj in arranged_objs:
            if obj.type == 'MESH' or obj.type == 'ARMATURE':
                if obj.parent:
                    if obj.parent.type != 'ARMATURE':
                        self.add_child_entry(object_entries, obj, obj.parent.name)
                    else:
                        object_entries.append(self.object_entry(obj))
                        object_entries[-1].rig = obj.parent
                else:
                    object_entries.append(self.object_entry(obj))
            has_arm |= (obj.type == 'ARMATURE')

        prefab_file = open(self.fd + self.fn + ".prefab", "wb")
        self.write(prefab_file, '{\n  "object":{\n')
        self.write(prefab_file, indent(2) + '"name.String":"' + self.fn + '",\n')
        self.write(prefab_file, indent(2) + '"position.Vec3":[ "0", "0", "0" ],\n')
        self.write(prefab_file, indent(2) + '"rotation.Quat":[ "1", "0", "0", "0" ],\n')
        self.write(prefab_file, indent(2) + '"scale.Vec3":[ "1", "1", "1" ],\n')
        if has_arm:
            self.write(prefab_file, indent(2) + '"components.ObjGroup":{\n')
            self.write(prefab_file, indent(3) + '"Animator":{}\n')
            self.write(prefab_file, indent(2) + '},\n')
        self.write(prefab_file, indent(2) + '"children.ObjGroup":{\n')
        
        self.export_entries(prefab_file, object_entries, indent(2))
        indent0 = ""
        
        self.write(prefab_file, indent(2) + "}\n")
        self.write(prefab_file, indent(1) + '},\n' + indent(1) + '"readonly":"1"\n}')
        
        printlog("-------export end--------")

    def add_child_entry(self, object_entries, obj, pnm):
        for e in object_entries:
            if e.obj.name == pnm:
                e.children.append(self.object_entry(obj))
                return True
            if self.add_child_entry(e.children, obj, pnm):
                return True
        return False
    
    def export_entries(self, prefab_file, entries, indent0_):
        global indent0
        indent0 = indent0_
        n = len(entries)
        for i, e in enumerate(entries, 1):
            printlog("object " + str(i) + "/" + str(n) + ": " + e.obj.name)
            mesh_is_skinned = False
            self.write(prefab_file, indent(1) + '"object":{\n')
            self.write(prefab_file, indent(2) + '"name.String":"' + e.obj.name + '",\n')
            if e.obj.parent_type == "BONE":
                for b in e.obj.parent.data.bones:
                    if b.name == e.obj.parent_bone:
                        tree = self.getobjtree(b)
                        break
                tree.extend(self.getobjtree(e.obj.parent))
                tree.reverse()
                self.write(prefab_file, indent(2) + '"parent.SceneObject":{ ' + ', '.join(tree) + ' },\n')
                #self.write(prefab_file, indent(2) + '"parent_bone.String":"' + e.obj.parent_bone + '",\n')
                
            elif e.rig:
            #    self.write(prefab_file, indent(2) + '"rig.String":"' + e.rig.name + '",\n')
                mesh_is_skinned = True
            poss = e.obj.location
            self.write(prefab_file, indent(2) + '"position.Vec3":[ "{:f}", "{:f}", "{:f}" ],\n'.format(poss[0], poss[2], -poss[1]))
            rott = e.obj.rotation_euler.to_quaternion()
            self.write(prefab_file, indent(2) + '"rotation.Quat":[ "{:f}", "{:f}", "{:f}", "{:f}" ],\n'.format(rott[0], rott[1], rott[3], -rott[2]))
            scll = e.obj.scale
            self.write(prefab_file, indent(2) + '"scale.Vec3":[ "{:f}", "{:f}", "{:f}" ],\n'.format(scll[0], scll[2], scll[1]))

            if e.obj.type == 'MESH':
                mesh_is_shaped = e.obj.data.shape_keys and len(e.obj.data.shape_keys.key_blocks)
                self.write(prefab_file, indent(2) + '"components.ObjGroup":{\n' + indent(3) + '"MeshRenderer":{\n')
                self.write(prefab_file, indent(4) + '"mesh.Asset":{"Mesh":"' + self.relfd + e.obj.name + '.mesh' + '"},\n')
                self.write(prefab_file, indent(4) + '"modifiers.ItemGroup":{\n')
                if mesh_is_shaped:
                    self.write(prefab_file, indent(5) + '"shape.ItemGroup":{}' + (',\n' if mesh_is_skinned else '\n'))
                if mesh_is_skinned:
                    self.write(prefab_file, indent(5) + '"skin.ItemGroup":{\n')
                    self.write(prefab_file, indent(6) + '"rig.Component":{\n')
                    tree = self.getobjtree(e.rig)
                    tree.reverse()
                    self.write(prefab_file, indent(7) + '"object":{ ' + ', '.join(tree) + ' }, "component":"Rig"\n')
                    self.write(prefab_file, indent(6) + '}\n')
                    self.write(prefab_file, indent(5) + '}\n')
                self.write(prefab_file, indent(4) + '},\n')
                self.write(prefab_file, indent(4) + '"materials.ItemGroup":{\n')
                ml = len(e.obj.data.materials)
                for j in range(max(ml-1, 0)):
                    self.write(prefab_file, indent(5) + '"' + str(j) + '.Asset":{"Material":"def.material"},\n')
                self.write(prefab_file, indent(5) + '"' + str(ml) + '.Asset":{"Material":"def.material"}\n')
                self.write(prefab_file, indent(4) + '}\n')
                self.write(prefab_file, indent(3) + '}\n')
                self.export_mesh(self.fd + e.obj.name + '.mesh', e.obj)
            elif e.obj.type == 'ARMATURE':
                self.write(prefab_file, indent(2) + '"components.ObjGroup":{\n' + indent(3) + '"Rig":{\n')
                self.write(prefab_file, indent(4) + '"armature.Asset":{"Armature":"' + self.relfd + e.obj.name + '.armature' + '"}\n')
                self.write(prefab_file, indent(3) + '}\n')
                self.export_armature(self.fd + e.obj.name + '.armature', e.obj)
                self.export_anim(self.fd, e.obj)

            if len(e.children) > 0:
                self.write(prefab_file, indent(2) + '},\n')
                self.write(prefab_file, indent(2) + '"children.ObjGroup":{\n')
                self.export_entries(prefab_file, e.children, indent(2))
                indent0 = indent0_
                self.write(prefab_file, indent(2) + '}\n')
            else:
                self.write(prefab_file, indent(2) + '}\n')
            if i < n:
                self.write(prefab_file, indent(1) + '},\n')
            else:
                self.write(prefab_file, indent(1) + '}\n')

    def export_mesh(self, path, obj):
        arma = None
        for mod in obj.modifiers:
            if mod.type == 'ARMATURE':
                arma = mod
                obj.modifiers.remove(mod)
                break

        bpy.context.view_layer.objects.active = obj

        m = obj.data

        num_shapes = len(m.shape_keys.key_blocks) if m.shape_keys else 0
        if num_shapes:
            obj.modifiers.clear()
        else:
            while (len(obj.modifiers) > 0):
                bpy.ops.object.modifier_apply(apply_as='DATA', modifier=obj.modifiers[0].name)

        #extract and clean vertex data
        verts = m.vertices
        vrts = [] #(vert_id, uv)
        loop2vrt = [] #int
        _vrts2 = []
        vrts2 = []
        
        if m.uv_layers:
            if len(m.uv_layers) > 1:
                for uv2 in m.uv_layers[1].data:
                    _vrts2.append(uv2.uv)
                printlog (" exporting secondary UV maps")
            uid = 0
            vrtc = 0
            for i, uvl in enumerate(m.uv_layers[0].data):
                pr = (m.loops[uid].vertex_index, uvl.uv)
                try:
                    vid = vrts.index(pr)
                except ValueError:
                    vrts.append(pr)
                    loop2vrt.append(vrtc)
                    vrtc = vrtc + 1
                    if _vrts2:
                        vrts2.append(_vrts2[i])
                else:
                    loop2vrt.append(vid)
                uid = uid + 1
        else:
            for i in range(len(verts)):
                vrts.append((i, 0))
            loop2vrt = list(range(len(verts)))

        vrtsz = len(vrts)

        #write data
        printlog ("!writing to: " + path)
        file = open(path, "wb")
        self.write(file, self.SIGNATURE + "\x00V")

        #V size4 [(3xvert4 3xnorm4) array]
        file.write(struct.pack("<i", vrtsz))
        for vrt in vrts:
            v = verts[vrt[0]]
            file.write(struct.pack("<fff", v.co[0], v.co[2], -v.co[1]))
            file.write(struct.pack("<fff", v.normal[0], v.normal[2], -v.normal[1]))
        
        #S size1 [name NULL [3xdelta4 array] array]
        if num_shapes:
            shapes = m.shape_keys.key_blocks
            self.write(file, "S")
            file.write(struct.pack("<B", num_shapes - 1))
            is_first = True
            for shp in shapes:
                if is_first:
                    is_first = False
                else:
                    self.write(file, shp.name + "\x00")
                    for vrt in vrts:
                        v = verts[vrt[0]].co
                        vs = shp.data[vrt[0]].co
                        delta = vs - v
                        file.write(struct.pack("<fff", delta[0], delta[2], -delta[1]))
            #bpy.ops.object.shape_key_remove(all=True)
        
        #obj.modifiers.new("EdgeSplit", 'EDGE_SPLIT')
        #obj.modifiers["EdgeSplit"].split_angle = 1.0472 #60 degrees
        #obj.modifiers["EdgeSplit"].use_edge_sharp = True
        #obj.modifiers.new("Triangulate", 'TRIANGULATE')
        #bpy.ops.object.modifier_apply(apply_as='DATA', modifier="Triangulate")

        #F size4 [{material1 3xface4} array]
        tris = []
        for poly in m.polygons:
            self.triangulate(poly, tris)
        self.write(file, "F")
        file.write(struct.pack("<i", len(tris)))
        for tri in tris:
            file.write(struct.pack("<B", tri[0]))
            file.write(struct.pack("<iii", loop2vrt[tri[1][0]], loop2vrt[tri[1][1]], loop2vrt[tri[1][2]]))
        
        #U size1 [uv04 array] ([uv14 array])
        if m.uv_layers:
            self.write(file, "U")
            file.write(struct.pack("<B", 2 if vrts2 else 1))
            for v in vrts:
                file.write(struct.pack("<ff", v[1][0], v[1][1]))
            if vrts2:
                for v in vrts2:
                    file.write(struct.pack("<ff", v[0], v[1]))
        
        #G size1 [groupName NULL array] (for each vert)[{groupSz1 [groupId1 groupWeight4 array]} array]
        if len(obj.vertex_groups):
            self.write(file, "G")
            file.write(struct.pack("<B", len(obj.vertex_groups)))
            for grp in obj.vertex_groups:
                self.write(file, grp.name + "\x00")
            for vrt in vrts:
                vert = verts[vrt[0]]
                file.write(struct.pack("<B", len(vert.groups)))
                for grp in vert.groups:
                    file.write(struct.pack("<Bf", grp.group, grp.weight))
            
        file.close()
        
    def arrange_objs(self, objs):
        oo = []
        for o in objs:
            if o.parent == None:
                self.do_arrange_objs(o, oo)
        return oo
    
    def do_arrange_objs(self, o, oo):
        oo.append(o)
        for c in o.children:
            self.do_arrange_objs(c, oo)
        
    def export_armature(self, path, arm):
        global indent0
        indent0_ = indent0
        bpy.context.view_layer.objects.active = arm
        bpy.ops.object.mode_set(mode='EDIT')
        bones = []
        for b in arm.data.edit_bones:
            if not b.parent:
                bones.append(b)

        printlog ("!writing to: " + path)
        file = open(path, "wb")
        self.write(file, '{\n' + indent(1) + '"armature":{\n')
        nb = len(bones)
        for i, b in enumerate(bones, 1):
            self.export_bone(file, b, indent(2), i == nb)
        indent0 = indent0_
        self.write(file, indent(1) + '}\n}')
        bpy.ops.object.mode_set(mode='OBJECT')

    def export_bone(self, file, bone, indent0_, last):
        global indent0
        indent0 = indent0_
        self.write(file, indent(1) + '"' + bone.name + '":{\n')
        vec = bone.head
        self.write(file, indent(2) + '"head":[ "{:f}", "{:f}", "{:f}" ],\n'.format(vec[0], vec[2], -vec[1]))
        vec = bone.tail
        self.write(file, indent(2) + '"tail":[ "{:f}", "{:f}", "{:f}" ],\n'.format(vec[0], vec[2], -vec[1]))
        vec = bone.z_axis
        self.write(file, indent(2) + '"front":[ "{:f}", "{:f}", "{:f}" ],\n'.format(vec[0], vec[2], -vec[1]))
        self.write(file, indent(2) + '"connected":' + ('"1"' if bone.use_connect else '"0"'))
        nc = len(bone.children)
        if nc > 0:
            self.write(file, ',\n' + indent(2) + '"children":{\n')
            for i, b in enumerate(bone.children, 1):
                self.export_bone(file, b, indent(3), i == nc)
            indent0 = indent0_
            self.write(file, indent(2) + '}\n')
        else:
            self.write(file, '\n')
        self.write(file, indent(1) + ('}\n' if last else '},\n'))

    def export_anim(self, path, arm):
        arm.data.pose_position = "POSE"
        bones = arm.pose.bones

        fullnames = []
        for b in bones:
            fullnames.append(self.bonefullname(b))

        for action in bpy.data.actions:
            if len(action.fcurves) == 0:
                continue
            if action.id_root != "OBJECT":
                continue

            #bones = []
            #fullnames = []
            #for b, f in zip(_bones, _fullnames):
            #    for c in action.fcurves:
            #        if c.data_path.startswith('pose.bones["' + b.name + '"]'):
            #            bones.append(b)
            #            fullnames.append(f)
            #            break
            #if len(bones) == 0:
            #    continue
            
            arm.animation_data.action = action
            
            frange = action.frame_range
            fr0 = int(frange[0])
            fr1 = int(frange[1])
            
            mats = []
            for f in range(fr0, fr1 + 1):
                self.scene.frame_set(f)
                m = []
                
                for bn in bones:
                    mat = self.bonelocalmat(bn)
                    rot = mat.to_quaternion()
                    if not bn.parent:
                        drot = Quaternion([1, 0, 0], 3.14159)
                        rot = drot @ rot
                    m.append([mat.to_translation(), rot, mat.to_scale()])
                mats.append(m)

            _path = path + arm.name + "_" + action.name + ".animclip"
            printlog ("!writing to: " + _path)
            file = open(_path, "wb")
            self.write(file, "ANIM")
            file.write(struct.pack("<H", len(bones)*3))
            file.write(struct.pack("<ii", fr0, fr1))
            
            # name(str) \0 -1(uint) nc(byte) isquat(byte)
            #   [value(floatxnc)]x(frameend-framestart+1)
            for i, bfn in enumerate(fullnames):
                self.write(file, bfn + "@T")
                file.write(b"\x00\xff\xff\xff\xff\x03\x00")
                for m in mats:
                    res = m[i][0]
                    if (bones[i].parent):
                        file.write(struct.pack("<fff", res[0], -res[2], res[1]))
                    else:
                        file.write(struct.pack("<fff", res[0], res[2], -res[1]))
                self.write(file, bfn + "@R")
                file.write(b"\x00\xff\xff\xff\xff\x04\x01")
                for m in mats:
                    res = m[i][1]
                    if (bones[i].parent):
                        file.write(struct.pack("<ffff", res[1], -res[3], res[2], res[0]))
                    else:
                        file.write(struct.pack("<ffff", res[1], res[3], -res[2], res[0]))
                self.write(file, bfn + "@S")
                file.write(b"\x00\xff\xff\xff\xff\x03\x00")
                for m in mats:
                    res = m[i][2]
                    file.write(struct.pack("<fff", res[0], res[2], res[1]))
            
            file.close()

    def bonefullname (self, bone):
        if bone.parent:
            return self.bonefullname(bone.parent) + "/" + bone.name
        else:
            return bone.name

    def bonelocalmat (self, bone):
        if bone.parent:
            return bone.parent.matrix.inverted() @ bone.matrix
        else:
            return bone.matrix

    def getobjtree (self, obj):
        res = []
        while obj:
            res.append('"' + obj.name + '":"0"')
            obj = obj.parent
        return res

    def triangulate (self, src, res):
        for i in range(1, len(src.loop_indices)-1):
            res.append((src.material_index, [src.loop_indices[0], src.loop_indices[i], src.loop_indices[i+1]]))

    def cleandir(self, path):
        if os.path.isdir(path):
            shutil.rmtree(path)
        os.makedirs(path)

    def write(self, file, str):
        file.write(str.encode())

if __name__ == "__main__":
    CE_Exporter().execute()
