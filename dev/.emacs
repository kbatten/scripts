(setq backup-inhibited t)
(setq-default indent-tabs-mode nil)
(setq column-number-mode t)

(setq mode-require-final-newline nil)

;; disable << nonsense in shell scripts
(add-hook 'sh-set-shell-hook 'my-disable-here-document)
(defun my-disable-here-document ()
  (local-set-key "<" 'self-insert-command))
