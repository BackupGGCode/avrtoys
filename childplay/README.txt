@Name: Child sound player
@Language: C (gcc)
@Author: Ivan A-R <ivalrom@gmail.com> (http://iar.spb.ru)
@Status: in research

Simple sound player. Recomended for child's toys.

Based on ATtiny2313 and AT26DFxxx (!!TODO!! currently on AT45DFxxx).

Features:
  - low cost (ATtiny2313 ~$1, AT26DF081A ~$1)
  - terminal access !!TODO!!
  - transfer samples by xmodem
  - configurable events  !!TODO!!
  - adpmc encoded (?)

Terminal commands: !!TODO!!

  - 'L' - list samples
  - 'S<n>' - store sample in device with alpha-numeric code <n>. Transfer by xmodem protocol.
  - 'D<n>' - delete sample
  - 'E!' - erase all samples
  - 'P<n>' - play sample
  - 'L<n>' - load sample (?)


