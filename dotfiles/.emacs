(setq backup-inhibited t)
(setq-default indent-tabs-mode nil)
(setq column-number-mode t)

(setq mode-require-final-newline nil)

;; disable << nonsense in shell scripts
(add-hook 'sh-set-shell-hook 'my-disable-here-document)
(defun my-disable-here-document ()
  (local-set-key "<" 'self-insert-command))

;; indent whole buffer
;; http://emacsblog.org/2007/01/17/indent-whole-buffer/
(defun iwb ()
  "indent whole buffer"
  (interactive)
  (delete-trailing-whitespace)
  (indent-region (point-min) (point-max) nil)
  (untabify (point-min) (point-max)))
