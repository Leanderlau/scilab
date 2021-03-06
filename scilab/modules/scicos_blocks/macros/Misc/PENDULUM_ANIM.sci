//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//
///withoutPrompt
function [x,y,typ]=PENDULUM_ANIM(job,arg1,arg2)
    // Animation of the cart-pendulum problem
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        dstate=model.dstate
        while %t do
            [ok,plen,csiz,phi,xmin,xmax,ymin,ymax,exprs]=scicos_getvalue(..
            msprintf(_("Set %s block parameters"), "PENDULUM_ANIM"),..
              [_("Pendulum length") ; 
               _("Cart size (square side)") ; 
               _("Slope"); "Xmin" ; "Xmax" ; "Ymin" ; "Ymax"], ..
            list("vec",1,"vec",1,"vec",1,"vec",1,"vec",1,"vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end
            mess=[]
            if plen<=0|csiz<=0 then
                mess=[mess;_("Pendulum length and cart size must be > 0.");" "]
                ok=%f
            end
            if ymin>=ymax then
                mess=[mess;_("Ymax > Ymin required");" "]
                ok=%f
            end
            if xmin>=xmax then
                mess=[mess;_("Xmax > Xmin required");" "]
                ok=%f
            end

            if ~ok then
                message(mess)
            else
                rpar=[plen;csiz;phi;xmin;xmax;ymin;ymax]
                model.rpar=rpar;
                graphics.exprs=exprs;
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        plen=2;
        csiz=2;
        phi=0;
        xmin=-5;
        xmax=5;
        ymin=-5;
        ymax=5
        model=scicos_model()
        model.sim=list("anim_pen",5)
        model.in=[1;1]
        model.evtin=1
        model.dstate=0
        model.rpar=[plen;csiz;phi;xmin;xmax;ymin;ymax]
        model.blocktype="d"
        model.dep_ut=[%f %f]

        exprs=string(model.rpar)
        gr_i=[]
        x=standard_define([3 3],model,exprs,gr_i)
    end

endfunction ///\withPrompt{}
