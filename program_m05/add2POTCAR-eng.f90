!  Translated from the portuguese version add2POTCAR2.f90
!  Translated from the portuguese version add2POTCAR2.f90
!  Translated from the portuguese version add2POTCAR2.f90
!  Translated from the portuguese version add2POTCAR2.f90
!  Translated from the portuguese version add2POTCAR2.f90
module fouriermod
contains
   function fourier(ca,nupon,r,VAR,inicio,inicial,fim,final)
   integer::nupon,i,iniciovez
   real(8)::ca,r(nupon),VAR(nupon),fim,final,fourier,inicio,inicial
   fourier=0.d0
   iniciovez=0
   do i=1,nupon
      if(r(i).le.inicio)cycle
      if(iniciovez.eq.0)then
         fourier=fourier+&
            (VAR(i)*dsin(ca*r(i))+inicial*dsin(ca*dsin(ca*inicio)))*(r(i)-inicio)/2.d0
         iniciovez=1
         cycle
      endif
      if(r(i).gt.fim)then
         fourier=fourier+&
            (VAR(i)*dsin(ca*r(i))+final*dsin(ca*dsin(ca*fim)))*(fim-r(i))/2.d0
         exit
      endif
      fourier=fourier+&
         (VAR(i)*dsin(ca*r(i))+VAR(i-1)*dsin(ca*r(i-1)))*(r(i)-r(i-1))/2.d0
   enddo
   end function fourier

end module fouriermod

Program add2POTCAR
use fouriermod
   character(20):: Vatomo, Vion, POTCAR, POTCARnew, rfile,INPUT
   character(40)::forkmax,forVVK
   character(120)::linha
   integer:: JUMPS,nupon,i,nuk,n
   real(8)::r(2001),patomo(2001),pion(2001),VAR(2001),kmax,VVK(1501),CUT,ca,&
      inicio,inicial,fim,final,amplitude
   real(8),parameter:: alpha=13.605803_8,beta=0.52917706_8,pi=3.141592654_8
!
   open(unit=12,file='add2POTCAR.out')
   write(*,*)' Enter the name of the input file. If * then reads from screen'
   read(*,*)INPUT
   INPUT=TRIM(INPUT)
   if(INPUT.ne.'*') open(unit=15,file=INPUT)
   write(12,'(2a)')INPUT,' = INPUT file'
   if(INPUT.eq.'*')then
      write(*,*)' Enter the name of the radii file'
      read(*,*)rfile
   else
      read(15,'(a20)')rfile
      rfile=TRIM(rfile)
   endif
   write(12,'(2a)')rfile,' = file of radii'
   if(INPUT.eq.'*')then
      write(*,*)' Enter JUMPS and the number of points'
      read(*,*)JUMPS,nupon
   else
      read(15,*)JUMPS,nupon
   endif
   write(12,'(2i5,a)')JUMPS,nupon,' = JUMPS,nupon'
   open(unit=11,file=rfile)
   do i=1,JUMPS;read(11,*);enddo
   read(11,*)(r(i),i=1,nupon)
   close (11)
!
   if(INPUT.eq.'*')then
      write(*,*)' Enter the name of the atomic potential file'
      read(*,*)Vatomo
   else
      read(15,'(a20)')Vatomo
      Vatomo=TRIM(Vatomo)
   endif
   write(12,'(2a)')Vatomo,' = atomic potential file'
   open(unit=11,file=Vatomo)
   if(INPUT.eq.'*')then
      write(*,*)' Enter JUMPS'
      read(*,*)JUMPS
   else
      read(15,*)JUMPS
   endif
   write(12,'(i5,a)')JUMPS,' = JUMPS'
   do i=1,JUMPS;read(11,*);enddo
   read(11,*)(patomo(i),i=1,nupon)
   close (11)
!
   if(INPUT.eq.'*')then
      write(*,*)' Enter the name of the -1/2 ionic potential file'
      read(*,*)Vion
   else
      read(15,'(a20)')Vion
      Vion=TRIM(Vion)
   endif
   write(12,'(2a)')Vion,' = -1/2 ionic potential file'
   open(unit=11,file=Vion)
   if(INPUT.eq.'*')then
      write(*,*)' Enter JUMPS'
      read(*,*)JUMPS
   else
      read(15,*)JUMPS
   endif
   write(12,'(i5,a)')JUMPS,' = JUMPS'
   do i=1,JUMPS;read(11,*);enddo
   read(11,*)(pion(i),i=1,nupon)
   close (11)
!
   if(INPUT.eq.'*')then
      write(*,*)' Enter parameters of cutting function (n,CUT,amplitude)'
! amplitude should be always = 1
      read(*,*)n,CUT,amplitude
   else
! amplitude should be always = 1
      read(15,*)n,CUT,amplitude
   endif
   write(12,'(i4,2f10.6,a)')n,CUT,amplitude,' = n   CUT  amplitude'
   do i=1,nupon
      VAR(i)=0.d0
      if(r(i).lt.CUT) &
         VAR(i)=4.d0*pi*alpha*beta**3*(1.d0-r(i)**n/CUT**n)**3*&
          (pion(i)-patomo(i))*amplitude
   enddo
!
   if(INPUT.eq.'*')then
      write(*,*)' Enter the name of the POTCAR file'
      read(*,*)POTCAR
   else
      read(15,'(a20)')POTCAR
      POTCAR=TRIM(POTCAR)
   endif
   write(12,'(2a)')POTCAR,' = POTCAR file'
   open(unit=11,file=POTCAR)
   if(INPUT.eq.'*')then
      write(*,*)' Enter JUMPS and number of k''s'
      read(*,*)JUMPS,nuk
   else
      read(15,*)JUMPS,nuk
   endif
   write(12,'(2i5,a)')JUMPS,nuk,' = JUMPS, number of k''s'
   if(INPUT.eq.'*')then
      write(*,*)' Enter the name of the new POTCAR to be constructed'
      read(*,*)POTCARnew
   else
      read(15,'(a20)')POTCARnew
      POTCARnew=TRIM(POTCARnew)
   endif
   write(12,'(2a)')POTCARnew,' =  POTCARnew file'
   open(unit=13,file=POTCARnew)
   do i=1,JUMPS;read(11,'(a)')linha;write(13,'(a)')TRIM(linha);enddo
   if(INPUT.eq.'*')then
      write(*,*)' Enter the format to read/write Kmax in file POTCAR'
      read(*,*)forkmax
   else
      read(15,'(a40)')forkmax
      forkmax=TRIM(forkmax)
   endif
   write(12,'(2a)')forkmax,' = format of Kmax in file POTCAR'
   read(11,*)kmax
   write(13,FMT=forkmax)kmax
   if(INPUT.eq.'*')then
      write(*,*)' Enter format of V(k) in file POTCAR'
      read(*,*)forVVK
   else
      read(15,'(a40)')forVVK
      forVVK=TRIM(forVVK)
      close (15)
   endif
   write(12,'(2a)')forVVK,' = format of V(k) in file POTCAR'
   read(11,*)(VVK(i),i=1,nuk)
!
!  Fourier transform
   ca=0.d0
   do i=1,nuk
      ca=ca+kmax/nuk
      VVK(i)=VVK(i)+fourier(beta*ca,nupon,r,VAR,0.d0,0.d0,CUT,0.d0)/beta/ca
   enddo
!
! write new POTCAR
   write(13,FMT=forVVK)(VVK(i),i=1,nuk)
100 read(11,'(a)',end=200,err=200)linha
    write(13,'(a)')TRIM(linha)
    go to 100
200 stop
end Program add2POTCAR

