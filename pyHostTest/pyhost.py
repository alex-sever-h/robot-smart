'''
Created on Nov 30, 2012

@author: alexs
'''
import pygame
from pygame.locals import *
import bluetooth

target_name = "roboBT"
target_address = None
sock = 0



def search_roboBT():
    #nearby_devices = bluetooth.discover_devices()
    #for bdaddr in nearby_devices:
    #    if target_name == bluetooth.lookup_name( bdaddr ):
    #        target_address = bdaddr
    #        break
    
    global target_address 
    
    target_address = '00:12:6F:27:EF:A8'

    if target_address is not None:
        print ("found target bluetooth device with address ", target_address)
    else:
        print ("could not find target bluetooth device nearby")
        exit()
    pass


def connect_roboBT():
    global sock 
    sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
    sock.connect((target_address, 1))
    pass
 
def disconnect_roboBT():
    global sock
    sock.close()
    pass
 
def init_keyloop():
    pygame.init()
    pygame.display.set_mode((64, 48))
    pygame.display.set_caption('Pygame Caption')
    pygame.mouse.set_visible(0)
    pass


def mainloop():
    done = False
    recv_buffer = ""
    while not done:
        for event in pygame.event.get():
            if (event.type == KEYDOWN):
                if (event.key == K_UP):
                    # event
                    go_forward()
                if (event.key == K_DOWN):
                    #print event
                    go_backward()
                if (event.key == K_RIGHT):
                    #print event
                    go_rotate_right()
                if (event.key == K_LEFT):
                    #print event
                    go_rotate_left()
            if (event.type == KEYUP):
                if (event.key == K_UP or event.key == K_DOWN or event.key == K_LEFT or event.key == K_RIGHT):
                    #print event
                    go_stop()
            if (event.type == KEYUP) or (event.type == KEYDOWN):
                #print event
                if (event.key == K_ESCAPE):
                    done = True
        
        recv_buffer = recv_buffer + sock.recv(5)
        splitcmd = recv_buffer.split("\n", 1)
         
        if len(splitcmd) >= 2:
            if(splitcmd[0] == "RDY"):
                print splitcmd[0]
            if(splitcmd[0] == "ACK"):
                print splitcmd[0]
            recv_buffer = splitcmd[1]


def convToString(left, right):
    retstring = ""
    
    if left >= 0:
        retstring = retstring + "+" + repr(left).zfill(3)
    else:
        retstring = retstring + repr(left).zfill(4)
    
    if right >= 0:
        retstring = retstring + "+" + repr(right).zfill(3)
    else:
        retstring = retstring + repr(right).zfill(4)    
        
    return retstring

def m_lr_command(left, right):
    goString = "M_LR"    
    command = goString + convToString(left, right) + "\r"    
    return command

def m_fb_command(distance):
    goString="M_FT"

    if(distance>=0):
    	command = goString + "+" + repr(distance).zfill(4)
    else:
    	command = goString + repr(distance).zfill(5)
    
    print command + "\n"
    
    return command + "\r"

def m_rot_command(distance):
    goString="M_RT"

    if(distance>=0):
        command = goString + "+" + repr(distance).zfill(4)
    else:
        command = goString + repr(distance).zfill(5)
    
    print command + "\n"
    
    return command + "\r"


def go_forward():
    global sock
    sock.send(m_fb_command(9)) 
    print "forward"
    pass

def go_backward():
    global sock
    sock.send(m_fb_command(-9))
    print "backward"
    pass

def go_rotate_left():
    global sock
    sock.send(m_rot_command(+9))
    print "leftward"
    pass

def go_rotate_right():
    global sock
    sock.send(m_rot_command(-9))
    print "rightward"
    pass


def go_backward_left():
    global sock
    sock.send(m_lr_command(-500, -999))
    pass

def go_backward_right():
    global sock
    sock.send(m_lr_command(-999, -500))
    pass

def go_stop():
    global sock
    #sock.send(m_lr_command(0, 0))
    print "stop"
    pass



if __name__ == '__main__':
    
    search_roboBT()
    connect_roboBT()
 
    init_keyloop()
    mainloop()
    
    disconnect_roboBT()
    pass
 
                
