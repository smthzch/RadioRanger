$fn=50;

// switch holder
switch_l=26.5;
switch_d=20;
switch_h=13.5;
switch_t=1;
// enclosure
length=80; //150 //40
width=40;
wall_thickness=4;

//  ESP32 holder
module pinhole(w, d, h, o, do){
    difference(){
        translate([-o/2, -o/2, 0]) cube([w+o, d+do+o, h]);
        cube([w, d + do, h]);
    };
};

module esp_holder(){
    w=16.8;
    d=0.65;
    h=9;
    o=2;
    do = 0.2;
    
    pinhole(w, d, h, o, do);
    translate([0, 15.78 - d, 0]) pinhole(w, d, h, o, do);
}

module switch(){
    difference(){
        // switch body
        cube([switch_d, switch_l, switch_h]);
        // hollow
        translate([0, switch_t, switch_t])
        cube([
            switch_d-2*switch_t, 
            switch_l-2*switch_t, 
            switch_h-2*switch_t
        ]);
        // wire holes
        translate([switch_h/3, 0, 0])
        cube([switch_d/3, 1, switch_h]);
        // wire holes
        translate([switch_h/3, switch_l-switch_t, 0])
        cube([switch_d/3, 1, switch_h]);
    }
}

// enclosure
module shell(){   
    r=width/2;
    difference(){
        //main body
        union(){
            //center mass
            rotate([-90,0,0])
            cylinder(h=length,r=r);
            // end lobe 1
            sphere(r);
            // end lobe 2
            translate([0, length, 0])
            sphere(r);
        }
        // cutoff top
        translate([-r, -r, r - r/4])
        cube([2*r, length+2*r, 2*r]);
        // cutoff bottom
        translate([-r, -r, -3*r + r/4])
        cube([2*r, length+2*r, 2*r]);
    }
}

module hollow_shell(){
    difference(){
        // scale from origin so need to translate everything to be centered
        translate([0,length/2,0]) // return to original position
        difference(){
            translate([0, -length/2, 0]) // center it
            shell();
            //hollow it
            scale([
                1 - (wall_thickness / width), 
                1 - (wall_thickness / (length+width)), 
                1 - (wall_thickness / (3*width/4))
            ])
            translate([0, -length/2, 0]) // center it before scale
            shell();
        }
        // mounting hole
        translate([0, 0, -width/2])
        cylinder(h=width, d=3.2);
        // mounting hole
        translate([0, length, -width/2])
        cylinder(h=width, d=3.2);       
    }
    // internal components
    // switch
    translate([-width/2+switch_t+wall_thickness/2-1, 3, -switch_h/2])
    switch();
    //esp holder
    translate([-width/3, switch_l + 18, -width/3])
    esp_holder();
}

module enclosure(section){
    if(section=="top"){
        difference(){
            //shell
            hollow_shell();
            //remove bottom
            translate([-width/2, -width/2, -width])
            cube([width, length+width, width]);
            
            // switch hole
            hole_l=16;
            hole_h=5;
            translate([-width/2, 26.5/2 - hole_l/2 + 3, -hole_h/2])
            cube([wall_thickness, hole_l, hole_h]);
            // usb hole
            translate([-width/2,switch_l+18+1.5,0])
            cube([wall_thickness, 13, 5]);
        }
        
    }if(section=="bottom"){
        difference(){
            //shell
            hollow_shell();
            //remove top
            translate([-width/2, -width/2, 0])
            cube([width, length+width, width]);
            // switch hole
            hole_l=16;
            hole_h=5;
            translate([-width/2, 26.5/2 - hole_l/2 + 3, -hole_h/2])
            cube([wall_thickness, hole_l, hole_h]);
            // usb hole
            translate([-width/2,switch_l+18+1.5,-2.2])
            cube([wall_thickness, 13, 2.2]);
        }
        // add mounting holes
        //mounting hole 1
        translate([0, 0, -3*width/8 + wall_thickness/2])
        difference(){
            cylinder(h=3*width/4 - wall_thickness, d=6);
            cylinder(h=3*width/4 - wall_thickness, d=3.2);
        }
        //mounting hole 2
        translate([0, length, -3*width/8 + wall_thickness/2])
        difference(){
            cylinder(h=3*width/4 - wall_thickness, d=6);
            cylinder(h=3*width/4 - wall_thickness, d=3.2);
        }

    }
}
section="bottom";
enclosure(section=section);
//enclosure(section="top");

