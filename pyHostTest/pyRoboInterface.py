'''
Created on May 20, 2013

@author: alseh
'''


import Tkinter
import bluetooth


class BluShtutz:
    def __init__(self):
        self.target_name = "roboBT"
        self.target_address = None
        self.sock = 0
        self.connected_state = False
        
        self.recv_buffer = ""
        
        
        if self.search_roboBT(False):
            print "bt address found: " + self.target_address
            self.connect_roboBT()
            
        if self.connected_state:
            print "bt connected to:  " + self.target_address
            
                
    def connect_roboBT(self):
        self.sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
        print self.sock.connect((self.target_address, 1))
        self.connected_state = True
#             print "connected to:"
     
    def disconnect_roboBT(self):
        self.sock.close()
        self.connected_state = False
        
    def send(self, data):
        if self.connected_state:
            self.sock.send(data)
            
    def receive(self):
        if self.connected_state:
            return self.sock.recv(128)
        else:
            return None
        
    def poll_sensors(self):
        self.recv_buffer += self.receive()
                        
        splitcmd = self.recv_buffer.split("\n", 1)
        
        if len(splitcmd) >= 2:
#             print "1111:  " + splitcmd[0] + "   :1111"
#             print "2222:  " + splitcmd[1] + "   :2222"
            self.recv_buffer = splitcmd[1]
            return splitcmd[0]
    
    def search_roboBT(self, search=False):
        if search:
            nearby_devices = bluetooth.discover_devices()
            for bdaddr in nearby_devices:
                if self.target_name == bluetooth.lookup_name( bdaddr ):
                    self.target_address = bdaddr
                    break
        else:
            self.target_address = '00:12:6F:27:EF:A8'
                
        if self.target_address is not None:
            return True
        else:
            return False

    def gen_m_lr_command(self, left, right):
        goString = "M_LR"    
        retstring = ""
        
        if left >= 0:
            retstring = retstring + "+" + repr(left).zfill(3)
        else:
            retstring = retstring + repr(left).zfill(4)
        
        if right >= 0:
            retstring = retstring + "+" + repr(right).zfill(3)
        else:
            retstring = retstring + repr(right).zfill(4)    
            
        retstring = goString + retstring + "\r"
            
        return retstring

    def go_forward(self):
        self.send(self.gen_m_lr_command(+999, +999))
    
    def go_rotate_left(self):
        self.send(self.gen_m_lr_command(-999, +999))
    
    def go_rotate_right(self):
        self.send(self.gen_m_lr_command(+999, -999))
    
    def go_backward(self):
        self.send(self.gen_m_lr_command(-999, -999))
        
    def go_stop(self):
        self.send(self.gen_m_lr_command(0, 0))


class MainApp:
    
    def __init__(self, master, blushtutz):
        self.master    = master
        self.blushtutz = blushtutz
        
        self.width  = 1280
        self.height = 720
                
        self.frame = Tkinter.Frame(master)
        self.frame.pack()

        frameButtons = Tkinter.Frame(master)
        frameButtons.pack(side=Tkinter.TOP)
        
        self.button = Tkinter.Button(frameButtons, text="QUIT", fg="red", command=self.exit)
        self.button.pack(side=Tkinter.LEFT)

        self.hi_there = Tkinter.Button(frameButtons, text="Hello", command=self.say_hi)
        self.hi_there.pack(side=Tkinter.LEFT)
        
        self.canvas = Tkinter.Canvas(self.frame, height=self.height, width=self.width)
        self.canvas.pack(side=Tkinter.BOTTOM)
        
        item = self.canvas.create_line(0, 0, 300, 300, tags="uno")
        self.canvas.itemconfig(item, tags=("one", "two"))
        self.canvas.addtag_withtag("three", "one")
        
        master.bind("<Left>",  self.go_left)
        master.bind("<Right>", self.go_right)
        master.bind("<Up>",    self.go_up)
        master.bind("<Down>",  self.go_down)
        master.bind("<Key>", self.go_stop)
        master.bind("<Escape>" ,  self.exit)
        #master.pack()
        
        master.after(50, self.poll_sensors)
                
        
    def go_left(self, event):
        
        print "go_left"
        self.blushtutz.go_rotate_left()
        
    def go_stop(self, event):
        
        print "go_left"
        self.blushtutz.go_stop()
            
    def go_right(self, event):
        
        print "go_right"
        self.blushtutz.go_rotate_right()
        
    def go_up(self, event):
        
        print "go_up"
        self.blushtutz.go_forward()
        
    def go_down(self, event):
        
        print "go_down"
        self.blushtutz.go_backward()    
        
    def create_sensor_area(self, x_origin, y_origin, length, angle_middle, color, span=30):
        
        
        x0 = x_origin - 2*length
        x1 = x_origin + 2*length
        y0 = y_origin - 2*length
        y1 = y_origin + 2*length
        
        angle_start  = 90 + angle_middle - span/2
        angle_extent = span
        
        return self.canvas.create_arc(x0, y0, x1, y1, start=angle_start, extent=angle_extent, fill=color)

    def exit(self, event):
        self.blushtutz.disconnect_roboBT()
        self.frame.quit()
        pass

    def say_hi(self):
        print "hi there, everyone!"
        
    def poll_sensors(self):
#         while True:
#             read = self.blushtutz.poll_sensors()
#             if read:
#                 self.draw_arc(read)
        self.master.after(50, self.poll_sensors)
        print self.blushtutz.receive()
        pass
        
    def draw_arc(self, read_string):
        if(read_string == None):
            return
        split = read_string.split(":")
        if(len(split) == 2):
            id  = split[0].strip()
            
            if(id == "R_R"):
                x = self.width/2 + 5
                y = self.height/2 + 10
                angle = 190
            if(id == "R_L"):
                x = self.width/2 - 5
                y = self.height/2 + 10
                angle = 170
            if(id == "F_R"):
                x = self.width/2 + 5
                y = self.height/2 - 10
                angle = -10
            if(id == "F_L"):
                x = self.width/2 - 5
                y = self.height/2 - 10
                angle = 10
            
            length = split[1].strip().strip("mm")
            length = int(length) / 10
            
            print id, " -> ", length
            
            arc1 = self.create_sensor_area(x, y, 1000, angle, "blue")
            arc2 = self.create_sensor_area(x, y, length,  angle, "red")
            

if __name__ == '__main__':
    
    root = Tkinter.Tk()
    blu  = BluShtutz()
    # blu  = None
    
    app = MainApp(root, blu)
    
    root.mainloop()
    
    
    
    
    
    
    
    