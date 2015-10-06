# A1b's Differences from A1:
#   Add the response to EOF
#   Add the situation for circle street
#   Change the recognize way of input from using split to using string scan
#   using sys.stderr/stdin/stdout.read/write instead of raw_input/print

import sys
# def global change
ar=[]
names=[]
vertexs_dict={}
edges=[]

# creat class street
class Street:
    name = ''
    point = []
    vertex = []
    edge = []
    cross = []
    def __init__(self,n,p):
        self.name = n
        self.point = p

# add a street
def a(name,point):
    if name in names:
        sys.stderr.write("Error: Already exist "+name+".\n")
    else:
        ar.append(Street(name,point))
        names.append(name)

# change a street
def c(name,point):
    if name not in names:
        sys.stderr.write("Error: 'c' specified for a street that does not exist.\n")
    else:
        c_temp_index=names.index(name)
        ar[c_temp_index].point=point

### remove a street
##def r(name):
##    if name not in names:
##       sys.stderr.write("Error: 'r' specified for a street that does not exist.\n")
##    else:
##        r_temp_index=names.index(name)
##        del names[r_temp_index]
##        del ar[r_temp_index]

#remove all
def r():
    global ar
    ar=[]
    global names
    names=[]
    global vertexs_dict
    vertexs_dict={}
    global edges
    edges=[]

# generate the graphic
def g():
    output=""
    all_vertex = []
    vertexs=[]
    global vertexs_dict
    for i in range(0,len(names)):
        calvertex(ar[i])
        all_vertex=all_vertex+ar[i].vertex
        for xx in ar[i].vertex:
            if xx not in vertexs:
                vertexs.append(xx)
    if len(vertexs_dict) == 0:  # first time run g()
        output=output+"V = "
        for i in range(0,len(vertexs)):
            vertexs_dict.update({i:vertexs[i]})
        output=output+str(len(vertexs))+"\n" #output of V
        keys_temp=vertexs_dict.keys()
        output=output+"E = {"
        for i in range(0,len(ar)):
            for j in range(0,len(ar[i].vertex)-1):
                temp=ar[i].vertex
                cccc=0
                for temp_c in ar[i].cross:
                    if (temp_c == temp[j]) or (temp_c == temp[j+1]):
                        cccc=1
                if cccc==1:
                    output=output+"<"+str(keys_temp[vertexs.index(temp[j])])+","+str(keys_temp[vertexs.index(temp[j+1])])+">,"
        output=output[:-1]
        output=output+"}\n"
    else:
        output=output+"V = "
        keys_temp=vertexs_dict.keys()
        values_temp=vertexs_dict.values()
        for i in range(0,len(vertexs)):
            if vertexs[i] in values_temp:
               continue
            else:
                vertexs_dict.update({len(vertexs_dict):vertexs[i]})
                keys_temp=vertexs_dict.keys()
                values_temp=vertexs_dict.values()
        output=output+str(len(vertexs_dict))+"\n"
        keys_temp=vertexs_dict.keys()
        values_temp=vertexs_dict.values()
        output=output+"E = {"
        for i in range(0,len(ar)):
            for j in range(0,len(ar[i].vertex)-1):
                temp=ar[i].vertex
                cccc=0
                for temp_c in ar[i].cross:
                    if (temp_c == temp[j]) or (temp_c == temp[j+1]):
                        cccc=1
                if cccc==1:
                    output=output+"<"+str(keys_temp[values_temp.index(temp[j])])+","+str(keys_temp[values_temp.index(temp[j+1])])+">,"
        output=output[:-1];
        output=output+"}\n"
    sys.stdout.write(output)
    sys.exit()

# get the cross of two segement lines
# line1's point (p1x,p1y) (p2x,p2y)
# line2's point (p3x,p3y) (p4x,p4y)
# return 0 means no cross
# if there is a cross, return (x,y) (turn to type int)
def cross((p1x,p1y),(p2x,p2y),(p3x,p3y),(p4x,p4y)):
    den= ((p4y-p3y)*(p2x-p1x)-(p4x-p3x)*(p2y-p1y))*1.0
    if den == 0:
        return 0
    else:
        ua=1.0*((p4x-p3x)*(p1y-p3y)-(p4y-p3y)*(p1x-p3x))/den
        ub=1.0*((p2x-p1x)*(p1y-p3y)-(p2y-p1y)*(p1x-p3x))/den
        if ua >= 0 and ua <= 1 and ub >= 0 and ub <= 1:
            x=round(1.0*(p3x+ub*(p4x-p3x)),2)
            y=round(1.0*(p3y+ub*(p4y-p3y)),2)
            if x == int(x):
                x=int(x)
            if y == int(y):
                y=int(y)
            return (x,y) # with one number after the point if not int
        else:
            return 0

# whether a point is in a segement line or not
# return 0 not in
# return 1 in
def pointinline((m,n),(x1,y1),(x2,y2)):
    if ((y2-n)*(x1-m) == (y1-n)*(x2-m)) and (y2-n)*(y1-n) <= 0 and (x2-m)*(x1-m) <= 0 and (m,n) != (x1,y1) and (m,n) != (x2,y2):
        return 1
    else:
        return 0

# calculate the vertexs of each street based on the derived crosses and add them to .vertex
def calvertex(street1):
    temp_vertex= []
    temp_cross= []
    temp1=street1.point
    for x1 in range(0,len(temp1)-1):
        tt_cross=[] # check each short line
        for i in range(0,len(ar)):
            temp2=ar[i].point
            for x2 in range(0,len(temp2)-1):
                if temp1 == temp2: # cross to its self
                    if len(temp1) < 3:
                        temp=0
                    elif (x2 - x1) > 1 or (x1 - x2) >1:
                        temp=cross(temp1[x1],temp1[x1+1],temp2[x2],temp2[x2+1])
                    else:
                        temp=0
                else:
                    temp=cross(temp1[x1],temp1[x1+1],temp2[x2],temp2[x2+1])
                
                if temp == 0: # no cross
                    pass
                elif temp_vertex == []:
                    temp_cross.append(temp)
                    tt_cross.append(temp)
                    temp_vertex.append(temp1[x1])
                    if temp not in temp_vertex:
                        temp_vertex.append(temp)
                    if temp1[x1+1] not in temp_vertex:
                        temp_vertex.append(temp1[x1+1])
                else:
                    if temp1[x1] not in temp_vertex:
                        temp_vertex.append(temp1[x1])
                    if temp not in tt_cross:
                        tt_cross.append(temp)
                        temp_cross.append(temp)
                        if pointinline(temp,temp_vertex[-1],temp_vertex[-2]) == 1:
                            temp_vertex.insert(-1,temp)
                        elif temp != temp_vertex[-1] and temp!= temp_vertex[-2]:
                            temp_vertex.append(temp)
                    if temp1[x1+1] not in temp_vertex:
                        temp_vertex.append(temp1[x1+1])
    street1.vertex=temp_vertex
    street1.cross=temp_cross

# turn string:(x,y) to tuple (x,y)
def strtotu(inputstr):
    if inputstr.startswith("(")==1 and inputstr.endswith(")")==1:
        temp=inputstr[1:len(inputstr)-1].split(",")
        if len(temp)==2 and numbercr(temp[0])==1 and numbercr(temp[1])==1:
            x=int(temp[0])
            y=int(temp[1])
            return (x,y)
        else:
            return False
    else:#wrong format
        return False

#whether a string can turn to number
def numbercr(x):
    try:
        x=int(x)
        return True
    except ValueError:
        return False

# turn string to standard form
def stringop(inputstr):
    i=0 # index of the string
    command = " " # save the command type
    savename = -1
    namedone = 0
    savepoint = -1
    name=''
    pointstr=''
    point=[]
    while i < len(inputstr):
        if command == "a" or command == "c" or command == "r" or command == "g":
            if (inputstr[i] == " " or inputstr[i] == "\t")and savename == -1:
                i=i+1
            elif inputstr[i] == "\n":
                if command == " ": # all space, so do nothing
                    return 2
                elif command == 'a' and namedone ==1 and point != [] and savepoint == -1:
                    a(name,point)
                elif command == 'c'and namedone ==1 and point != [] and savepoint == -1:
                    c(name,point)
                elif command == 'r'and namedone ==0 and point == []:
                    r()
                elif command == 'g'and point == [] and namedone ==0:
                    g()
                else:
                    return 0
                if i == len(inputstr)-1:
                    return 2
                i = i+1
                command = " " # save the command type
                savename = -1
                namedone = 0
                savepoint = -1
                name=""
                pointstr=""
                point=[]
            elif inputstr[i] == "\"":
                if namedone == 1:
                    return 0
                savename = savename * (-1)
                i=i+1
                if savename == -1:
                    namedone = 1
            elif savename == 1: #start save name
                name=name+inputstr[i]
                i=i+1
            elif namedone == 1 and inputstr[i] == '(' and savepoint == -1:
                savepoint = savepoint * (-1)
                pointstr = pointstr + inputstr[i]
                i=i+1
            elif namedone == 1 and inputstr[i] == ')' and savepoint == 1:
                savepoint = savepoint * (-1)
                pointstr = pointstr + inputstr[i]
                i=i+1
                if strtotu(pointstr) != False:
                    point.append(strtotu(pointstr))
                    pointstr=""
                else:
                    return 0
            elif namedone == 1 and savepoint == 1:
                pointstr = pointstr + inputstr[i]
                i=i+1
            else:
                return 0
        elif inputstr[i] == 'a' or inputstr[i] == 'c' or inputstr[i] == 'r' or inputstr[i] == 'g': # verify the command type
            command = inputstr[i]
            i = i+1
        elif inputstr[i] == " " or inputstr[i] == "\t": # if with a space or tab, ignore it
            i = i+1
        elif inputstr[i] == "\n":
            i=i+1
        else:# return 0: wrong format
            return 0
        
# main program
while True:
    line=sys.stdin.readline()
    if not line: # Check EOF
        break
    check=stringop(line)
    if check == 0: # print error of wrong format
        sys.stderr.write("Error: Wrong Input Format!\n")
    elif check == 2:# do noting
        continue


